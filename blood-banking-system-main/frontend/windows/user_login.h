#include "../gtkInjector.h"
#include "../socket/socketInjector.h"

#include <gtk/gtk.h>

#include "../../shared/entities/entities.h"
#include "../../shared/routes.h"

GtkEntry *userLoginPhoneInput;
GtkEntry *userLoginPasswordInput;
GtkWidget *button;
GtkWidget *registerButton;

static void on_user_register_button_clicked(void) {
	gtk_widget_hide(userLoginWindow);
	gtk_widget_show(userRegistrationWindow);
}

static void on_user_login_button_clicked(void) {
  const char* phone = gtk_entry_get_text(userLoginPhoneInput);
  const char* pass = gtk_entry_get_text(userLoginPasswordInput);

  if (strlen(phone) == 0 || strlen(pass) == 0) {
    return;
  }

  printf("Phone: %s\n", phone);
  printf("Password: %s\n", pass);

  struct Request request;
  struct Response response;

  sprintf(request.route.module, AUTH_MODULE);
  sprintf(request.route.method, USER_LOGIN_METHOD);

  sprintf(request.phoneNumber, "%s", phone);
  sprintf(request.password, "%s", pass);

  sendAll(socketClientId, &request, sizeof(request), 0);
  recv(socketClientId, &response, sizeof(response), MSG_WAITALL);

  printf("[USER_LOGIN] Status: %d\n", response.status);
  printf("[USER_LOGIN] Name: %s\n", response.data.user.name);

  // if auth success
  if (response.status == 1) {
    sprintf(userPhone, "%s", phone);
    
    gtk_widget_hide(userLoginWindow);
	  gtk_widget_show(userWindow);
  } else {
    
  }
}

void initUserLoginWindow() {
  userLoginWindow = GTK_WIDGET(gtk_builder_get_object(builder, "user_login"));

  userLoginPhoneInput = GTK_ENTRY(gtk_builder_get_object(builder, "user_phone"));
  userLoginPasswordInput = GTK_ENTRY(gtk_builder_get_object(builder, "password"));
  button = GTK_WIDGET(gtk_builder_get_object(builder,"Login_button"));
  registerButton = GTK_WIDGET(gtk_builder_get_object(builder,"reg_btn"));

  g_signal_connect(button, "clicked", G_CALLBACK(on_user_login_button_clicked), NULL);
  g_signal_connect(registerButton, "clicked", G_CALLBACK(on_user_register_button_clicked), NULL);
}
