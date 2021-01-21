#include "../gtkInjector.h"

#include <gtk/gtkx.h>

typedef enum {
  ID,
  BLOODTYPE,
  ACTION,
  STATUS,
  NUM_COLUMNS
} Column_type;

GtkBox *userBox;
GtkStack *userStack;
GtkWidget *adminLogOutButton;
GtkStackSwitcher *userSwitcher;
GtkWidget *getGrid;
GtkWidget *donateGrid;
GtkWidget *historyGrid;
GtkTreeView *tree;
GtkTreeModel *model;

static GtkTreeModel *create_and_fill_model (void){
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

static void on_admin_logout_button_clicked(void) {
	gtk_widget_hide(adminWindow);
	gtk_widget_show(initialWindow);
}

void initAdminWindow() {
	userWindow = GTK_WIDGET(gtk_builder_get_object(builder, "admin_page"));

  userLogOutButton = GTK_WIDGET(gtk_builder_get_object(builder, "logout_button_admin"));

  tree = GTK_WIDGET(gtk_builder_get_object(builder,"TreeView"));

	getGrid = GTK_WIDGET(gtk_builder_get_object(builder, "get_switch"));
	donateGrid = GTK_WIDGET(gtk_builder_get_object(builder, "donate_switch"));
	historyGrid = GTK_WIDGET(gtk_builder_get_object(builder, "history_switch"));

  userBox = GTK_BOX(gtk_builder_get_object(builder, "user_box"));
  userStack = GTK_STACK(gtk_builder_get_object(builder, "pages"));
  adminSwitcher = GTK_STACK_SWITCHER(gtk_builder_get_object(builder, "switch"));

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