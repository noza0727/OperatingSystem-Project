#include "../gtkInjector.h"

#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <gtk/gtkx.h>
#include <math.h>
#include <ctype.h>

GtkWidget	*window;
GtkWidget	*box;
GtkWidget	*grid;
GtkWidget	*button;
GtkWidget	*label;
GtkWidget	*radio_btn1;
GtkWidget	*radio_btn2;

int isAdminSelected = 1;

static void onContinueClicked(void) {
	gtk_widget_hide(initialWindow);

	if (isAdminSelected == 1) {
		gtk_widget_show(adminLoginWindow);
	} else {
		gtk_widget_show(userLoginWindow);
	}
}

void initInitialWindow() {
	initialWindow = GTK_WIDGET(gtk_builder_get_object(builder, "initial_page"));

	box = GTK_WIDGET(gtk_builder_get_object (builder, "choose_role_box"));
	grid = GTK_WIDGET(gtk_builder_get_object (builder, "choose_role_grid"));
	button = GTK_WIDGET(gtk_builder_get_object (builder, "role_cont_btn"));
	label = GTK_WIDGET(gtk_builder_get_object (builder, "role_logo_label"));

	radio_btn1 = GTK_WIDGET(gtk_builder_get_object (builder, "admin_radio_btn"));
	radio_btn2 = GTK_WIDGET(gtk_builder_get_object (builder, "user_radio_btn"));

	g_signal_connect(button, "clicked", G_CALLBACK(onContinueClicked), NULL);
}

void on_button_clicked (GtkButton *b) {
     gtk_label_set_text (GTK_LABEL(label), (const gchar*) "Blood Bank");
}

void on_admin_radio_btn_toggled(GtkRadioButton *b) {
	isAdminSelected = 1;
	gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b));
}

void on_user_radio_btn_toggled(GtkRadioButton *b) {
	isAdminSelected = 0;
	gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b));
}
