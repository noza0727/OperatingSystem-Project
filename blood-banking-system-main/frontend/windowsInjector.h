#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>

#include "gtkInjector.h"
#include "./windows/initial.h"
#include "./windows/user_login.h"
#include "./windows/admin_login.h"
#include "./windows/user_registration.h"
#include "./windows/user.h"
#include "./windows/doctor.h"
// #include "./windows/admin.h"

extern void injectViews() {
	builder = gtk_builder_new();

	gtk_builder_add_from_file(builder, "./glade/project_ui.glade", NULL);

	initUserWindow();
	initInitialWindow();
	initUserLoginWindow();
	initAdminLoginWindow();
	initUserRegistrationWindow();
	initDoctorWindow();
	// initAdminWindow();

	gtk_builder_connect_signals(builder, NULL);
	g_object_unref(builder);
}

