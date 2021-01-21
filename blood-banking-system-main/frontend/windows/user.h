#include "../gtkInjector.h"
#include "../socket/socketInjector.h"

#include "../../shared/entities/entities.h"
#include "../../shared/routes.h"

#include <stdio.h>
#include <time.h>
#include <gtk/gtkx.h>

typedef enum {
  ID,
  BLOODTYPE,
  ACTION,
  STATUS,
  NUM_COLUMNS
} Column_type;

GtkLabel *userInfoName;
GtkLabel *userInfoPhone;
GtkLabel *userInfoMail;
GtkLabel *userInfoBlood;
GtkLabel *userInfoId;
GtkLabel *userInfoRegion;

GtkWidget *userDonateButton;
GtkWidget *userLogOutButton;

GtkLabel *userDonateLabel;

GtkBox *userBox;
GtkStack *userStack;
GtkStackSwitcher *userSwitcher;
GtkWidget *getGrid;
GtkWidget *donateGrid;
GtkWidget *historyGrid;
GtkTreeView *tree;
GtkTreeModel *model;

int get_day_of_year() {
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  return tm.tm_yday;
}

int calculateDaysLeft(char *lastDateOfDonation, int afterDays){
    
    int year, month, day;
    
    sscanf(lastDateOfDonation, "%d-%d-%d", &year, &month, &day);
    
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    int currentDay = tm->tm_yday+1;
    int currentMonth = tm->tm_mon+1;
    int currentYear = tm->tm_year+1900;
    
    int dayToVisitDoy = getDoyAfterGivenTime(lastDateOfDonation, afterDays);
    int currentDoy = daysInGivenDate(currentYear, currentMonth, currentDay);
    int differnce = dayToVisitDoy - currentDoy;
    return differnce;
}

int getDoyAfterGivenTime(char* date, int addDays){

    int year, month, day;
    
    sscanf(date, "%d-%d-%d", &year, &month, &day);
    
    int doy = daysInGivenDate(year, month, day);
    int addedDoy = doy + addDays;
    
    return addedDoy;
}

int daysInGivenDate(int year, int mon, int day){
    int days_in_feb = 28, doy;    // day of year

    doy = day;

    // check for leap year
    if( (year % 4 == 0 && year % 100 != 0 ) || (year % 400 == 0) )
    {
        days_in_feb = 29;
    }

    switch(mon)
    {
        case 2:
            doy += 31;
            break;
        case 3:
            doy += 31+days_in_feb;
            break;
        case 4:
            doy += 31+days_in_feb+31;
            break;
        case 5:
            doy += 31+days_in_feb+31+30;
            break;
        case 6:
            doy += 31+days_in_feb+31+30+31;
            break;
        case 7:
            doy += 31+days_in_feb+31+30+31+30;
            break;            
        case 8:
            doy += 31+days_in_feb+31+30+31+30+31;
            break;
        case 9:
            doy += 31+days_in_feb+31+30+31+30+31+31;
            break;
        case 10:
            doy += 31+days_in_feb+31+30+31+30+31+31+30;            
            break;            
        case 11:
            doy += 31+days_in_feb+31+30+31+30+31+31+30+31;            
            break;                        
        case 12:
            doy += 31+days_in_feb+31+30+31+30+31+31+30+31+30;            
            break;                                    
    }

    return doy;
}

int compareDates(char *date, int daysDifference) {
  int year, month, day;

  sscanf(date, "%d-%d-%d", &year, &month, &day);

  int res = day - get_day_of_year();

  printf("Difference: %d\n", (res * res) / res);

  return (res * res) / res > daysDifference ? 1 : 0;
}

void checkUserDonationStatus() {
  gtk_widget_hide(userDonateButton);

  if (loggedUser.lastDonation.id == 0 || calculateDaysLeft(loggedUser.lastDonation.date, 30) <= 0) {
    gtk_widget_show(userDonateButton);
  } else if (strcmp(loggedUser.lastDonation.status, "approved") == 0) {
    gtk_label_set_text(userDonateLabel, "You have successfully donated blood, the next donation cycle is in 3 weeks. Thank you for contributing to a healthy future.");
  } else if (strcmp(loggedUser.lastDonation.status, "denied") == 0) {
    gtk_label_set_text(userDonateLabel, "Your test has been tested negative, please see your doctor and try to come back in a month");
  } else {
    gtk_label_set_text(userDonateLabel, "Your analyzes are being checked");
  }
}

void on_user_page_show() {
  struct Request request;
  struct Response response;

  sprintf(request.route.module, USER_MODULE);
  sprintf(request.route.method, USER_INFO_METHOD);

  sendAll(socketClientId, &request, sizeof(request), 0);
  recv(socketClientId, &response, sizeof(response), MSG_WAITALL);

  printf("[USER] Status: %d\n", response.status);
  printf("[USER] Name: %s\n", response.data.user.name);

  char temp[10];

  sprintf(temp, "%d", response.data.user.id);

  if (response.status == 1) {
    loggedUser = response.data.user;

    gtk_label_set_text(userInfoName, response.data.user.name);
    gtk_label_set_text(userInfoPhone, response.data.user.phoneNumber);
    gtk_label_set_text(userInfoMail, response.data.user.email);
    gtk_label_set_text(userInfoBlood, response.data.user.bloodType);
    gtk_label_set_text(userInfoId, temp);
    gtk_label_set_text(userInfoRegion, response.data.user.region);

    checkUserDonationStatus();
  } else {
    
  }
}



static void on_user_logout_button_clicked(void) {
	gtk_widget_hide(userWindow);
	gtk_widget_show(initialWindow);
}

static void on_user_donate_button_clicked(void) {
  struct Request request;
  struct Response response;

  sprintf(request.route.module, RECORD_MODULE);
  sprintf(request.route.method, DONATE_BLOOD_METHOD);

  sendAll(socketClientId, &request, sizeof(request), 0);
  recv(socketClientId, &response, sizeof(response), MSG_WAITALL);

  if (response.status == 1) {
    on_user_page_show();
  }
}

static GtkTreeModel *create_and_fill_model (void) {
  GtkListStore *store;
  GtkTreeIter iter;

  store = gtk_list_store_new(4, G_TYPE_STRING, G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING);

  gtk_list_store_append(store,&iter);
  gtk_list_store_set (store, &iter, ID, "1", BLOODTYPE, "A+", ACTION,"Get", STATUS, "Accepted", -1);
  gtk_list_store_append(store,&iter);
  gtk_list_store_set (store, &iter, ID, "1", BLOODTYPE, "A+", ACTION,"Get", STATUS, "Accepted", -1);
  gtk_list_store_append(store,&iter);
  gtk_list_store_set (store, &iter, ID, "1", BLOODTYPE, "A+", ACTION,"Get", STATUS, "Accepted", -1);

  return GTK_TREE_MODEL (store);
}

void initUserWindow() {
	userWindow = GTK_WIDGET(gtk_builder_get_object(builder, "user_page"));

  userInfoName = GTK_LABEL(gtk_builder_get_object(builder, "name_input"));
  userInfoPhone = GTK_LABEL(gtk_builder_get_object(builder, "mai_input"));
  userInfoMail = GTK_LABEL(gtk_builder_get_object(builder, "mail_input"));
  userInfoBlood = GTK_LABEL(gtk_builder_get_object(builder, "blood_input"));
  userInfoId = GTK_LABEL(gtk_builder_get_object(builder, "ID_input"));
  userInfoRegion = GTK_LABEL(gtk_builder_get_object(builder, "region_input"));

  userDonateButton = GTK_WIDGET(gtk_builder_get_object(builder, "donate_btn"));
  userLogOutButton = GTK_WIDGET(gtk_builder_get_object(builder, "user_logout"));

  userDonateLabel = GTK_LABEL(gtk_builder_get_object(builder, "user_donate_label"));

  tree = GTK_TREE_VIEW(gtk_builder_get_object(builder,"TreeView"));

	getGrid = GTK_WIDGET(gtk_builder_get_object(builder, "get_switch"));
	donateGrid = GTK_WIDGET(gtk_builder_get_object(builder, "donate_switch"));
	historyGrid = GTK_WIDGET(gtk_builder_get_object(builder, "history_switch"));

  userBox = GTK_BOX(gtk_builder_get_object(builder, "user_box"));
  userStack = GTK_STACK(gtk_builder_get_object(builder, "pages"));
  userSwitcher = GTK_STACK_SWITCHER(gtk_builder_get_object(builder, "switch"));

  g_signal_connect(userDonateButton, "clicked", G_CALLBACK(on_user_donate_button_clicked), NULL);
  g_signal_connect(userLogOutButton, "clicked", G_CALLBACK(on_user_logout_button_clicked), NULL);

  GtkTreeViewColumn *column1 = gtk_tree_view_column_new_with_attributes("ID",gtk_cell_renderer_text_new (),"text",ID,NULL);
	GtkTreeViewColumn *column2 = gtk_tree_view_column_new_with_attributes("Blood Type",gtk_cell_renderer_text_new (),"text",BLOODTYPE,NULL);  
	GtkTreeViewColumn *column3 = gtk_tree_view_column_new_with_attributes("Action",gtk_cell_renderer_text_new (),"text",ACTION,NULL);
	GtkTreeViewColumn *column4 = gtk_tree_view_column_new_with_attributes("Status",gtk_cell_renderer_text_new (),"text",STATUS,NULL);

	gtk_tree_view_column_set_expand(column1, TRUE);
	gtk_tree_view_column_set_expand(column2, TRUE);
	gtk_tree_view_column_set_expand(column3, TRUE);
	gtk_tree_view_column_set_expand(column4, TRUE);

	gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column1);
	gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column2);
	gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column3);
	gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column4);

	model = create_and_fill_model();
	gtk_tree_view_set_model(GTK_TREE_VIEW(tree), model);
}