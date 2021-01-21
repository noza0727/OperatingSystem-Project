#ifndef GTK_INJECTOR_H
#define GTK_INJECTOR_H

#include "../shared/entities/entities.h"

static GtkBuilder *builder;
static GtkWidget *initialWindow, *userLoginWindow, *adminLoginWindow, *userRegistrationWindow;
static GtkWidget *userWindow, *doctorWindow, *adminWindow, *editRecordWindow;
static char adminUsername[512], userPhone[512];
static struct User loggedUser;

#endif
