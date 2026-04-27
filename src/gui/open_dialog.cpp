#include <gtk/gtk.h>

// Структура для передачи данных в callback
typedef struct {
    GtkWidget *label;
    GtkFileDialog *dialog;
} AppData;

// Обработчик результата выбора файла
static void on_file_selected(GObject *source, GAsyncResult *result, gpointer user_data) {
    GtkFileDialog *dialog = GTK_FILE_DIALOG(source);
    AppData *data = (AppData *)user_data;
    GError *error = NULL;
    
    // Получаем выбранный файл (для GTK 4.14+ возвращает GFile*)
    GFile *file = gtk_file_dialog_open_finish(dialog, result, &error);
    
    if (file) {
        char *path = g_file_get_path(file);
        char *message = g_strdup_printf("Выбран файл: %s", path);
        gtk_label_set_text(GTK_LABEL(data->label), message);
        g_print("%s\n", message);
        
        g_free(path);
        g_free(message);
        g_object_unref(file);
    } else if (error) {
        if (!g_error_matches(error, GTK_DIALOG_ERROR, GTK_DIALOG_ERROR_CANCELLED)) {
            g_printerr("Ошибка: %s\n", error->message);
        }
        g_error_free(error);
    }
}

// Обработчик нажатия кнопки
static void on_open_clicked(GtkButton *button, gpointer user_data) {
    AppData *data = (AppData *)user_data;
    GtkWindow *window = GTK_WINDOW(gtk_widget_get_root(GTK_WIDGET(button)));
    
    // Показываем диалог асинхронно
    gtk_file_dialog_open(data->dialog, window, NULL, on_file_selected, data);
}

// Создание фильтра для текстовых файлов
static GtkFileFilter *create_text_filter(void) {
    GtkFileFilter *filter = gtk_file_filter_new();
    
    gtk_file_filter_set_name(filter, "Текстовые файлы");
    // Фильтр по MIME-типам
    gtk_file_filter_add_mime_type(filter, "text/plain");
    // Фильтр по расширениям файлов
    gtk_file_filter_add_pattern(filter, "*.txt");
    gtk_file_filter_add_pattern(filter, "*.md");
    gtk_file_filter_add_pattern(filter, "*.c");
    gtk_file_filter_add_pattern(filter, "*.h");
    
    return filter;
}

// Создание фильтра для изображений
static GtkFileFilter *create_image_filter(void) {
    GtkFileFilter *filter = gtk_file_filter_new();
    
    gtk_file_filter_set_name(filter, "Изображения");
    gtk_file_filter_add_mime_type(filter, "image/png");
    gtk_file_filter_add_mime_type(filter, "image/jpeg");
    gtk_file_filter_add_mime_type(filter, "image/jpg");
    gtk_file_filter_add_mime_type(filter, "image/gif");
    gtk_file_filter_add_pixbuf_formats(filter); // Добавляет все поддерживаемые GDK форматы
    
    return filter;
}

// Создание фильтра для PDF
static GtkFileFilter *create_pdf_filter(void) {
    GtkFileFilter *filter = gtk_file_filter_new();
    
    gtk_file_filter_set_name(filter, "PDF документы");
    gtk_file_filter_add_mime_type(filter, "application/pdf");
    gtk_file_filter_add_pattern(filter, "*.pdf");
    
    return filter;
}

static void on_activate(GtkApplication *app, gpointer user_data) {
    // Создаем главное окно
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Выбор файлов в GTK4");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);
    
    // Создаем контейнер
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_margin_start(box, 24);
    gtk_widget_set_margin_end(box, 24);
    gtk_widget_set_margin_top(box, 24);
    gtk_widget_set_margin_bottom(box, 24);
    gtk_window_set_child(GTK_WINDOW(window), box);
    
    // Создаем метку для отображения результата
    GtkWidget *label = gtk_label_new("Нажмите кнопку для выбора файла");
    gtk_label_set_xalign(GTK_LABEL(label), 0);
    gtk_widget_set_vexpand(label, TRUE);
    gtk_box_append(GTK_BOX(box), label);
    
    // Создаем кнопку
    GtkWidget *button = gtk_button_new_with_label("Выбрать файл");
    gtk_widget_set_halign(button, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(box), button);
    
    // Создаем диалог выбора файла
    GtkFileDialog *dialog = gtk_file_dialog_new();
    gtk_file_dialog_set_title(dialog, "Выберите файл");
    
    // --- НАСТРОЙКА ФИЛЬТРОВ ---
    // Создаем список фильтров (GListStore)
    GListStore *filters = g_list_store_new(GTK_TYPE_FILE_FILTER);
    
    // Добавляем фильтры
    GtkFileFilter *text_filter = create_text_filter();
    GtkFileFilter *image_filter = create_image_filter();
    GtkFileFilter *pdf_filter = create_pdf_filter();
    
    g_list_store_append(filters, text_filter);
    g_list_store_append(filters, image_filter);
    g_list_store_append(filters, pdf_filter);
    
    // Устанавливаем фильтры в диалог
    gtk_file_dialog_set_filters(dialog, G_LIST_MODEL(filters));
    
    // Устанавливаем фильтр по умолчанию (первый в списке будет активен)
    gtk_file_dialog_set_default_filter(dialog, text_filter);
    
    // Можно также создать "Все файлы" фильтр
    GtkFileFilter *all_filter = gtk_file_filter_new();
    gtk_file_filter_set_name(all_filter, "Все файлы");
    gtk_file_filter_add_pattern(all_filter, "*");
    g_list_store_append(filters, all_filter);
    
    // Освобождаем локальные ссылки на фильтры (GListStore теперь владеет ими)
    g_object_unref(text_filter);
    g_object_unref(image_filter);
    g_object_unref(pdf_filter);
    g_object_unref(all_filter);
    
    // Структура данных для передачи в callback
    AppData *data = g_new(AppData, 1);
    data->label = label;
    data->dialog = dialog;
    
    // Подключаем сигнал
    g_signal_connect(button, "clicked", G_CALLBACK(on_open_clicked), data);
    
    gtk_widget_show(window);
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("org.example.filefilter", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
    
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    
    return status;
}
