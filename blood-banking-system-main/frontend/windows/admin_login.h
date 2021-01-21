#include "../gtkInjector.h"
#include <gtk/gtk.h>

GtkEntry *adminLoginUsernameInput;
GtkEntry *adminLoginPasswordInput;
GtkWidget *button;

static void on_admin_login_button_clicked(void) {
  const char *username = gtk_entry_get_text(adminLoginUsernameInput);
  const char *pass = gtk_entry_get_text(adminLoginPasswordInput);

  if (strlen(username) == 0 || strlen(pass) == 0) {
    return;
  }

  printf("Username: %s\n", username);
  printf("Password: %s\n", pass);	

  struct Request request;
  struct Response response;

  sprintf(request.route.module, AUTH_MODULE);
  sprintf(request.route.method, ADMIN_LOGIN_METHOD);

  sprintf(request.username, "%s", username);
  sprintf(request.password, "%s", pass);

  sendAll(socketClientId, &request, sizeof(request), 0);
  recv(socketClientId, &response, sizeof(response), MSG_WAITALL);

  printf("[LOGIN_ADMIN] Status: %d\n", response.status);

  // if auth success
  if (response.status == 1) {
	  gtk_widget_hide(adminLoginWindow);
	  gtk_widget_show(doctorWindow);
  } else {

  }
}

void initAdminLoginWindow() {
  adminLoginWindow = GTK_WIDGET(gtk_builder_get_object(builder, "admin_login"));

  adminLoginUsernameInput = GTK_ENTRY(gtk_builder_get_object(builder, "admin_user"));
  adminLoginPasswordInput = GTK_ENTRY(gtk_builder_get_object(builder, "password1"));
  button = GTK_WIDGET(gtk_builder_get_object(builder,"Login_button1"));

  g_signal_connect(button, "clicked", G_CALLBACK(on_admin_login_button_clicked),NULL);
}
