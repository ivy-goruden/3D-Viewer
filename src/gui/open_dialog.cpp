#include "open_dialog.hpp"

#include <gtk/gtk.h>
#include <string>
#include "app_data.hpp"

OpenDialog::OpenDialog(GtkWindow* window_) {
    dialog = gtk_file_dialog_new();
    gtk_file_dialog_set_modal(dialog, true);
    gtk_file_dialog_set_title(dialog, "Выберите файл");

    filters = g_list_store_new(GTK_TYPE_FILE_FILTER);
    obj_filter = createObjFilter();
    text_filter = createTextFilter();
    all_filter = createAllFilesFilter();
    
    g_list_store_append(filters, obj_filter);
    g_list_store_append(filters, text_filter);
    g_list_store_append(filters, all_filter);

    gtk_file_dialog_set_filters(dialog, G_LIST_MODEL(filters));
    gtk_file_dialog_set_default_filter(dialog, obj_filter);
}

OpenDialog::~OpenDialog() {
    g_object_unref(obj_filter);
    g_object_unref(text_filter);
    g_object_unref(all_filter);
    g_object_unref(filters);
    g_object_unref(dialog);
}

void OpenDialog::setOnFileSelected(FileSelectedCallback callback) {
    onFileSelected = callback;
}

void OpenDialog::onOpenFileSelected(GObject *source, GAsyncResult *result, gpointer user_data) {
    OpenDialog *self = static_cast<OpenDialog*>(user_data);

    GError *error = NULL;
    GtkFileDialog *dialog = GTK_FILE_DIALOG(source);
    GFile *file = gtk_file_dialog_open_finish(dialog, result, &error);
    
    if (file) {
        char* cstr = g_file_get_path(file);
        self->openFileSelected(cstr);
        g_object_unref(file);
    } else if (error) {
        self->active = false;
        if (!g_error_matches(error, GTK_DIALOG_ERROR, GTK_DIALOG_ERROR_CANCELLED)) {
            g_printerr("Ошибка: %s\n", error->message);
        }
        g_error_free(error);
    }
}

GtkFileFilter* OpenDialog::createObjFilter() {
    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "Графические объекты");
    gtk_file_filter_add_pattern(filter, "*.obj");
    return filter;
}

GtkFileFilter* OpenDialog::createTextFilter() {
    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "Текстовые файлы");
    gtk_file_filter_add_mime_type(filter, "text/plain");
    gtk_file_filter_add_pattern(filter, "*.txt");
    gtk_file_filter_add_pattern(filter, "*.md");
    gtk_file_filter_add_pattern(filter, "*.c");
    gtk_file_filter_add_pattern(filter, "*.h");
    return filter;
}

GtkFileFilter* OpenDialog::createAllFilesFilter() {
    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "Все файлы");
    gtk_file_filter_add_pattern(filter, "*");
    return filter;
}

void OpenDialog::openFileSelected(char* p) {
    active = false;
    if (onFileSelected) {
        std::string selectedFile = p;
        onFileSelected(selectedFile);
    }
}

void OpenDialog::open() {
    active = true;
    gtk_file_dialog_open(dialog, window, NULL, onOpenFileSelected, this);
}

bool OpenDialog::isActive() {
    return active;
}
