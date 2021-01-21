#include "../gtkInjector.h"

#include <gtk/gtk.h>

typedef enum {
  D_T_ID,
  D_T_NAME,
  D_T_BLOODTYPE,
  D_T_DONATIONDATE,
  D_T_ACTION,
  D_T_NUM_COLUMNS
} Doctor_Column_type;

int selectedRecordId;
GtkEntry *recordBloodTypeInput;
GtkWidget *approveButton;
GtkWidget *declineButton;

GtkTreeView *treead;
GtkTreeModel *modelad;
GtkWidget *refresh_doctor_button;
GtkWidget *logout_button;
GtkTreeSelection *selection;

static GtkTreeModel *create_and_fill_doctor_model (struct Record *records) {
  GtkListStore *store;
  GtkTreeIter iter;

  store = gtk_list_store_new(5, G_TYPE_INT,G_TYPE_INT,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING);

  for (int i = 0; i < 10; i++) {
    if (records[i].id == NULL) {
      break;
    }

    gtk_list_store_append(store,&iter);
    gtk_list_store_set (
      store,
      &iter, D_T_ID,
      records[i].id,
      D_T_NAME,
      records[i].donor_id,
      D_T_BLOODTYPE,
      records[i].bloodType,
      D_T_DONATIONDATE,
      records[i].date,
      D_T_ACTION,
      records[i].status,
      -1
    );
  }

  return GTK_TREE_MODEL(store);
}

void on_doctor_page_show() {
  struct Request request;
  struct Response response;

  sprintf(request.route.module, RECORD_MODULE);
  sprintf(request.route.method, GET_PENDING_DONATES_METHOD);

  sendAll(socketClientId, &request, sizeof(request), 0);
  recv(socketClientId, &response, sizeof(response), MSG_WAITALL);

  printf("[DOCTOR] Status: %d\n", response.status);
  printf("[DOCTOR] Record: %d\n", response.data.records[0].id);

  if (response.status == 1) {
    modelad = create_and_fill_doctor_model(response.data.records);
    gtk_tree_view_set_model(GTK_TREE_VIEW (treead), modelad);
  }
}

static void onAdminLoginClicked(void) {
	gtk_widget_hide(doctorWindow);
  gtk_widget_show(initialWindow);
}

static void onDoctorRefreshClicked(void) {
  on_doctor_page_show();
}

static void onApproveButton(void) {
  const char* type = gtk_entry_get_text(recordBloodTypeInput);

  printf("[Approved] ID: %d, BType: %s\n", selectedRecordId, type);

  struct Request request;
  struct Response response;

  sprintf(request.route.module, RECORD_MODULE);
  sprintf(request.route.method, SET_ANALYSIS_RESULT_METHOD);

  request.id = selectedRecordId;

  sprintf(request.status, "%s", "approved");
  sprintf(request.bloodType, "%s", type);

  sendAll(socketClientId, &request, sizeof(request), 0);
  recv(socketClientId, &response, sizeof(response), MSG_WAITALL);

  on_doctor_page_show();
  gtk_widget_hide(editRecordWindow);
}

static void onDeclineButton(void) {
  const char* type = gtk_entry_get_text(recordBloodTypeInput);

  printf("[Approved] ID: %d, BType: %s\n", selectedRecordId, type);

  struct Request request;
  struct Response response;

  sprintf(request.route.module, RECORD_MODULE);
  sprintf(request.route.method, SET_ANALYSIS_RESULT_METHOD);

  request.id = selectedRecordId;

  sprintf(request.status, "%s", "denied");
  sprintf(request.bloodType, "%s", type);

  sendAll(socketClientId, &request, sizeof(request), 0);
  recv(socketClientId, &response, sizeof(response), MSG_WAITALL);

  on_doctor_page_show();
  gtk_widget_hide(editRecordWindow);
}

static void closeEdit(void) {
  gtk_widget_hide(editRecordWindow);
}

static void onTreeViewRowActivated(GtkTreeView *tree, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data){
  gtk_widget_show(editRecordWindow);

  GtkTreeModel *model = gtk_tree_view_get_model(tree);
  GtkTreeIter iter;

  if (gtk_tree_model_get_iter(model, &iter, path)) {
    int *recordId;

    gtk_tree_model_get(model, &iter, D_T_ID, &recordId, -1);

    selectedRecordId = recordId;
  }
}

void initDoctorWindow() {
  doctorWindow = GTK_WIDGET(gtk_builder_get_object(builder, "doctor_page"));
  editRecordWindow = GTK_WIDGET(gtk_builder_get_object(builder, "edit_window"));

  treead = GTK_TREE_VIEW(gtk_builder_get_object(builder,"treeview1"));

	refresh_doctor_button=GTK_WIDGET(gtk_builder_get_object(builder,"refresh_doctor_button"));
	logout_button=GTK_WIDGET(gtk_builder_get_object(builder,"doctor_logout"));

  recordBloodTypeInput = GTK_ENTRY(gtk_builder_get_object(builder, "blood_type"));
  approveButton = GTK_WIDGET(gtk_builder_get_object(builder,"approve_button"));
  declineButton = GTK_WIDGET(gtk_builder_get_object(builder,"not_approve_button"));

  GtkTreeViewColumn *column1=gtk_tree_view_column_new_with_attributes("ID",gtk_cell_renderer_text_new (),"text",D_T_ID,NULL);  
  GtkTreeViewColumn *column2=gtk_tree_view_column_new_with_attributes("Name",gtk_cell_renderer_text_new (),"text",D_T_NAME,NULL);  
	GtkTreeViewColumn *column3=gtk_tree_view_column_new_with_attributes("Blood Type",gtk_cell_renderer_text_new (),"text",D_T_BLOODTYPE,NULL);
	GtkTreeViewColumn *column4=gtk_tree_view_column_new_with_attributes("Donation date",gtk_cell_renderer_text_new (),"text",D_T_DONATIONDATE,NULL);
	GtkTreeViewColumn *column5=gtk_tree_view_column_new_with_attributes("Action",gtk_cell_renderer_text_new (),"text",D_T_ACTION,NULL);

	gtk_tree_view_column_set_expand(column1,TRUE);
	gtk_tree_view_column_set_expand(column2,TRUE);
	gtk_tree_view_column_set_expand(column3,TRUE);
	gtk_tree_view_column_set_expand(column4,TRUE);
	gtk_tree_view_column_set_expand(column5,TRUE);

	gtk_tree_view_append_column (GTK_TREE_VIEW (treead), column1);
	gtk_tree_view_append_column (GTK_TREE_VIEW (treead), column2);
	gtk_tree_view_append_column (GTK_TREE_VIEW (treead), column3);
	gtk_tree_view_append_column (GTK_TREE_VIEW (treead), column4);
	gtk_tree_view_append_column (GTK_TREE_VIEW (treead), column5);

	g_signal_connect(logout_button, "clicked", G_CALLBACK(onAdminLoginClicked),NULL);
	g_signal_connect(refresh_doctor_button, "clicked", G_CALLBACK(onDoctorRefreshClicked),NULL);

	g_signal_connect(approveButton, "clicked", G_CALLBACK(onApproveButton),NULL);
	g_signal_connect(declineButton, "clicked", G_CALLBACK(onDeclineButton),NULL);

	g_signal_connect(treead, "row-activated",(GCallback) onTreeViewRowActivated, NULL);
  g_signal_connect(G_OBJECT(editRecordWindow), "destroy", G_CALLBACK(closeEdit), NULL);
}
