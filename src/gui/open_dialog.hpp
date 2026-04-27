#ifndef OPENDIALOG_APP
#define OPENDIALOG_APP

#include "open_dialog.hpp"
#include <gtk/gtk.h>
#include <string>
#include <functional>

class OpenDialog {
  public:
    using FileSelectedCallback = std::function<void(const std::string& filepath)>;

  private:
    FileSelectedCallback onFileSelected;

  private:
    bool active;
    GtkWindow* window;
    GtkFileDialog *dialog;
    GListStore *filters;
    GtkFileFilter *obj_filter;
    GtkFileFilter *text_filter;
    GtkFileFilter *image_filter;
    GtkFileFilter *pdf_filter;
    GtkFileFilter *all_filter;
    void openFileSelect(char* p);
    static void onOpenFileSelected(GObject *source, GAsyncResult *result, gpointer user_data);
    GtkFileFilter* createObjFilter();
    GtkFileFilter* createTextFilter();
    GtkFileFilter* createImageFilter();
    GtkFileFilter* createPdfFilter();
    GtkFileFilter* createAllFilesFilter();

  public:
    OpenDialog(GtkWindow* window_);
    ~OpenDialog();
    void setOnFileSelected(FileSelectedCallback callback);
    bool isActive();
    void open();
};

#endif
