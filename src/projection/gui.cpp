#include "gui.hpp"
#include <time.h>
#include <gtk/gtk.h>
#include <iostream>
#include <filesystem>
#include <format>

#include "../include/globals.h"
#include "../include/obj_parser/obj_loader.hpp"
#include "../include/figure/figure.hpp"
#include "../include/affine_transformer/matrix.hpp"
#include "../include/affine_transformer/transformer.hpp"
#include "../include/grid.hpp"

#define TIME_BLOCK(code_block) do { \
    struct timespec start, end; \
    clock_gettime(CLOCK_MONOTONIC, &start); \
    code_block \
    clock_gettime(CLOCK_MONOTONIC, &end); \
    double time_spent = (end.tv_sec - start.tv_sec) + \
                        (end.tv_nsec - start.tv_nsec) / 1e9; \
    printf("Время выполнения: %f секунд\n", time_spent); \
} while(0)

void PrintMatrix(s21::matrix_t& m) {
    for (size_t i = 0; i < m.size(); i++) {
        std::cout << std::format("{},{},{}", m[i][0],m[i][1],m[i][2]) << std::endl;
    }
}

void DrawSomething(cairo_t* cr, GuiApp* gui) {
    std::cout << gui->tr.toString() << std::endl;
    gui->drawObject(cr, gui->shapeVert, gui->shapeFace, gui->tr);
}

s21::Point GuiApp::viewportToCanvas(double x, double y) {
    return toScreenPoint(s21::Point{x * Cw / Vw, y * Ch / Vh});
}

s21::Point GuiApp::projectVertex(s21::Point3d v) {
    return viewportToCanvas(v.x, v.y);
    // return viewportToCanvas(v.x * tr.scale / v.z, v.y * tr.scale / v.z);
}

void GuiApp::drawDot(cairo_t* cr, s21::Point p) {
    cairo_set_line_width(cr, 1);
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_arc(cr, p.x, p.y, 3, 0, 2 * M_PI);
    cairo_stroke(cr);
}

void GuiApp::drawLine(cairo_t* cr, s21::Point p1, s21::Point p2) {
    cairo_set_line_width(cr, 1);
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_move_to(cr, p1.x, p1.y);
    cairo_line_to(cr, p2.x, p2.y);
    cairo_stroke(cr);
}

void GuiApp::drawFace(cairo_t* cr, std::vector<int> face, s21::Vert_t& projected) {
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_move_to(cr, projected[face[0]].x, projected[face[0]].y);
    for (size_t i = 1; i < face.size(); i++) {
        cairo_line_to(cr, projected[face[i]].x, projected[face[i]].y);
    }
    cairo_close_path(cr);
    cairo_stroke(cr);
}

void GuiApp::drawObject(cairo_t *cr, s21::matrix_t &verts, s21::Poly_t &faces, s21::Transform tr) {
    s21::matrix_t vt;
    TIME_BLOCK({
        vt = applyTransform(verts, tr);
    
        s21::Vert_t projected;
        for (size_t i = 0; i < vt.size(); i++) {
            projected.push_back(projectVertex(vectorToPoint3d(vt[i])));
        }
        for (size_t i = 0; i < faces.size(); i++) {
            drawFace(cr, faces[i], projected);
        }
    });    
}

s21::Point3d GuiApp::vectorToPoint3d(std::vector<double> v) {
    return {v[0], v[1], v[2]};
}

s21::matrix_t GuiApp::applyTransform(s21::matrix_t &verts, s21::Transform tr) {
    // std::cout << "applyTransform tr " << tr.toString() << std::endl;

    s21::matrix_t transformed;
    transformed = s21::Transformer::Scale(tr.scale, tr.scale, tr.scale, verts);
    transformed = s21::Transformer::Rotate(tr.rotation_x, tr.rotation_y, tr.rotation_z, transformed);

    // PrintMatrix(transformed);
    s21::Bounds b = s21::Matrix::getBounds(transformed);
    // std::cout << "applyTransform scale b " << b.toString() << std::endl;
    if (b.minz < 0) this->tr.trans_z = std::abs(b.minz) + 10;
    transformed = s21::Transformer::Translate(tr.trans_x, tr.trans_y, this->tr.trans_z, transformed);
    return transformed;
}

s21::Point GuiApp::toScreenPoint(s21::Point canvasPoint) {
    return s21::Point{Cw / 2 + canvasPoint.x, Ch / 2 - canvasPoint.y};
}

s21::Point GuiApp::toCanvasPoint(s21::Point screenPoint) {
    return s21::Point{-Cw / 2 + screenPoint.x, Ch / 2 - screenPoint.y};
}

GuiApp::GuiApp() {
    key_data = g_new0(KeyData, 1);
    key_data->app = this;
    key_data->is_alt = false;
    key_data->is_ctrl = false;

    drag_data = g_new0(DragData, 1);
    drag_data->app = this;
    drag_data->offset_x = 0;
    drag_data->offset_y = 0;
    drag_data->is_dragging = false;

    d = 1;
    Cw = 1;
    Ch = 1;
    Vw = 1;
    Vh = 1;

    tr = {
        0.1,
        0, 0, 0,
        0, 0, 100
    };

    shapeVert.push_back({ 1,  1,  1,  1});
    shapeVert.push_back({-1,  1,  1,  1});
    shapeVert.push_back({-1, -1,  1,  1});
    shapeVert.push_back({ 1, -1,  1,  1});
    shapeVert.push_back({ 1,  1, -1,  1});
    shapeVert.push_back({-1,  1, -1,  1});
    shapeVert.push_back({-1, -1, -1,  1});
    shapeVert.push_back({ 1, -1, -1,  1});

    shapeFace.push_back({ 0, 1, 2 });
    shapeFace.push_back({ 0, 2, 3 });
    shapeFace.push_back({ 4, 0, 3 });
    shapeFace.push_back({ 4, 3, 7 });
    shapeFace.push_back({ 5, 4, 7 });
    shapeFace.push_back({ 5, 7, 6 });
    shapeFace.push_back({ 1, 5, 6 });
    shapeFace.push_back({ 1, 6, 2 });
    shapeFace.push_back({ 4, 5, 1 });
    shapeFace.push_back({ 4, 1, 0 });
    shapeFace.push_back({ 2, 6, 7 });
    shapeFace.push_back({ 2, 7, 3 });

    s21::ObjLoader loader = s21::ObjLoader();
    // loader.loadObjFile("assets/cube.obj");
    // loader.loadObjFile("assets/diamond.obj");
    loader.loadObjFile("assets/teapot.obj");
    // loader.loadObjFile("assets/beast.obj");
    // loader.loadObjFile("assets/trumpet.obj");
    // loader.loadObjFile("assets/shuttle.obj");
    // loader.loadObjFile("assets/Pottery Barn_Childrens Bedroom Nightstand N180226.obj");
    s21::Figure* figure  = new s21::Figure(loader);
    shapeVert = figure->getMatrix();
    shapeFace = figure->getPolygons();

    s21::Bounds b = s21::Matrix::getBounds(shapeVert);
    std::cout << b.toString() << std::endl;    
    
    shapeVert = s21::Transformer::Translate(-(b.maxx + b.minx)/2, -(b.maxy + b.miny)/2, -(b.maxz + b.minz)/2, shapeVert);

    b = s21::Matrix::getBounds(shapeVert);
    std::cout << b.toString() << std::endl;

    app = gtk_application_new("com.application", G_APPLICATION_DEFAULT_FLAGS);
}

GuiApp::~GuiApp() {
    g_free(key_data);
    g_free(drag_data);
    g_object_unref(app);
}

int GuiApp::run(int argc, char **argv) {
    g_signal_connect(app, "activate", G_CALLBACK(onActivate), this);
    return g_application_run(G_APPLICATION(app), argc, argv);
}

void GuiApp::onActivate(GtkApplication *app, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    self->activate(app);
}

void GuiApp::onOpenButtonClick(GtkButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
}

void GuiApp::onSaveButtonClick(GtkButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
}

void GuiApp::onResetButtonClick(GtkButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
}

void GuiApp::onXSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    double value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(self->xSpinnerButton));
    self->d = value;
    self->redraw();
    std::cout << "[onXSpinnerValueChanged] " << std::format("v{}\n", value);
}

void GuiApp::onYSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    double value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(self->ySpinnerButton));
}

void GuiApp::onZSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    double value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(self->zSpinnerButton));
}

void GuiApp::onShiftSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    double value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(self->shiftSpinnerButton));
}

void GuiApp::onShiftVSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    double value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(self->shiftVSpinnerButton));
}

void GuiApp::onZoomSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    double value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(self->zoomSpinnerButton));
}

void GuiApp::onLineSwitchActivate(GtkSwitch* sw, GParamSpec *pspec, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    bool active = gtk_switch_get_active(GTK_SWITCH(self->lineSwitch));
}

void GuiApp::onProjSwitchActivate(GtkSwitch* sw, GParamSpec *pspec, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    bool active = gtk_switch_get_active(GTK_SWITCH(self->projSwitch));
}

void GuiApp::onFillSwitchActivate(GtkSwitch* sw, GParamSpec *pspec, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    bool active = gtk_switch_get_active(GTK_SWITCH(self->fillSwitch));
}

void GuiApp::onVertModeToggled(GtkCheckButton* btn, GParamSpec *pspec, gpointer user_data) {
    if (!gtk_check_button_get_active(btn)) {
        return;
    }
    int vertMode = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(btn), "mode"));
    GuiApp *self = static_cast<GuiApp*>(user_data);
}

void GuiApp::onColorButtonClick(GtkButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
}

void GuiApp::onBgColorButtonClick(GtkButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
}

void GuiApp::onWeightSpinnerValueChanged(GtkSpinButton* btn, gpointer user_data) {
    GuiApp *self = static_cast<GuiApp*>(user_data);
    double value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(self->weightSpinnerButton));
}

void GuiApp::activate(GtkApplication* app) {
    GtkBuilder* builder = gtk_builder_new_from_file(ui_file.c_str());
    window = gtk_builder_get_object(builder, "main");
    openButton = gtk_builder_get_object(builder, "open_button");
    saveButton = gtk_builder_get_object(builder, "save_button");
    resetButton = gtk_builder_get_object(builder, "reset_button");
    colorButton = gtk_builder_get_object(builder, "color_button");
    bgcolorButton = gtk_builder_get_object(builder, "bgcolor_button");
    xSpinnerButton = gtk_builder_get_object(builder, "rotate_x_spinner");
    ySpinnerButton = gtk_builder_get_object(builder, "rotate_y_spinner");
    zSpinnerButton = gtk_builder_get_object(builder, "rotate_z_spinner");
    shiftSpinnerButton = gtk_builder_get_object(builder, "shift_spinner");
    shiftVSpinnerButton = gtk_builder_get_object(builder, "shift_v_spinner");
    zoomSpinnerButton = gtk_builder_get_object(builder, "zoom_spinner");
    weightSpinnerButton = gtk_builder_get_object(builder, "weight_spinner");
    paper = gtk_builder_get_object(builder, "paper");
    status_vert = gtk_builder_get_object(builder, "status_vert");
    status_file = gtk_builder_get_object(builder, "status_file");
    status_edges = gtk_builder_get_object(builder, "status_edges");
    statusLabel = gtk_builder_get_object(builder, "status_label");
    g_object_unref(builder);

    g_signal_connect(openButton, "clicked", G_CALLBACK(onOpenButtonClick), this);
    g_signal_connect(saveButton, "clicked", G_CALLBACK(onSaveButtonClick), this);
    g_signal_connect(resetButton, "clicked", G_CALLBACK(onResetButtonClick), this);
    g_signal_connect(colorButton, "clicked", G_CALLBACK(onColorButtonClick), this);
    g_signal_connect(bgcolorButton, "clicked", G_CALLBACK(onBgColorButtonClick), this);
    g_signal_connect(xSpinnerButton, "value_changed", G_CALLBACK(onXSpinnerValueChanged), this);
    g_signal_connect(ySpinnerButton, "value_changed", G_CALLBACK(onYSpinnerValueChanged), this);
    g_signal_connect(zSpinnerButton, "value_changed", G_CALLBACK(onZSpinnerValueChanged), this);
    g_signal_connect(shiftSpinnerButton, "value_changed", G_CALLBACK(onShiftSpinnerValueChanged), this);
    g_signal_connect(shiftVSpinnerButton, "value_changed", G_CALLBACK(onShiftVSpinnerValueChanged), this);
    g_signal_connect(zoomSpinnerButton, "value_changed", G_CALLBACK(onZoomSpinnerValueChanged), this);
    g_signal_connect(weightSpinnerButton, "value_changed", G_CALLBACK(onWeightSpinnerValueChanged), this);

    gtk_window_set_application(GTK_WINDOW(window), app);
    gtk_window_present(GTK_WINDOW(window));

    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(paper), on_draw_static, this, nullptr);

    GtkEventController *motion_controller = gtk_event_controller_motion_new();
    g_signal_connect(motion_controller, "motion", G_CALLBACK(on_motion), this);
    gtk_widget_add_controller(GTK_WIDGET(paper), motion_controller);

    // Создаем контроллер клавиатуры
    GtkEventController *key_controller = gtk_event_controller_key_new();
    // Подключаем сигналы
    g_signal_connect(key_controller, "key-pressed", G_CALLBACK(on_key_pressed), key_data);
    g_signal_connect(key_controller, "key-released", G_CALLBACK(on_key_released), key_data);
    // Добавляем контроллер на виджет
    gtk_widget_add_controller(GTK_WIDGET(window), key_controller);

    // Создаём жест для перетаскивания
    GtkGesture *drag_gesture = gtk_gesture_drag_new();
    // Подключаем сигналы
    g_signal_connect(drag_gesture, "drag-begin", G_CALLBACK(on_drag_begin), drag_data);
    g_signal_connect(drag_gesture, "drag-update", G_CALLBACK(on_drag_update), drag_data);
    g_signal_connect(drag_gesture, "drag-end", G_CALLBACK(on_drag_end), drag_data);
    // Добавляем контроллер на виджет
    gtk_widget_add_controller(GTK_WIDGET(paper), GTK_EVENT_CONTROLLER(drag_gesture));

    // Создаем контроллер прокрутки
    // GTK_EVENT_CONTROLLER_SCROLL_BOTH_AXES — отслеживаем обе оси
    GtkEventController *scroll_controller = GTK_EVENT_CONTROLLER(gtk_event_controller_scroll_new(
        GTK_EVENT_CONTROLLER_SCROLL_BOTH_AXES));
    // Подключаем сигнал "scroll" к нашему обработчику
    g_signal_connect(scroll_controller, "scroll", G_CALLBACK(on_scroll), this);
    // Добавляем контроллер к виджету
    gtk_widget_add_controller(GTK_WIDGET(paper), scroll_controller);

}

void GuiApp::on_motion(GtkEventControllerMotion *controller, gdouble x, gdouble y, gpointer user_data) {
    GdkModifierType state;
    GdkDevice *device = gtk_event_controller_get_current_event_device(GTK_EVENT_CONTROLLER(controller));
    g_print("Moving with left button pressed at (%.1f, %.1f)\n", x, y);
}

void GuiApp::on_draw_static(GtkDrawingArea* area, cairo_t* cr, int width, int height, gpointer user_data) {
    GuiApp* self = static_cast<GuiApp*>(user_data);
    self->onDraw(cr, width, height);
}

void GuiApp::onDragBegin() {
    tr_copy = tr;
    redraw();
}

void GuiApp::onDragUpdate() {
    if (!key_data->is_ctrl) {
        int angleX = floor(1.5 * 90 / Cw * drag_data->offset_y);
        int angleY = floor(1.5 * 90 / Cw * drag_data->offset_x);
        tr.rotation_x = tr_copy.rotation_x + angleX;
        tr.rotation_y = tr_copy.rotation_y + angleY;
        g_print("%d, %f, %f\n", Cw, angleX, angleY);
    } else {
        double dx = 1.0 * drag_data->offset_x * Vw / Cw / d;
        double dy = 1.0 * drag_data->offset_y * Vh / Ch / d;
        tr.trans_x = tr_copy.trans_x + dx;
        tr.trans_y = tr_copy.trans_y - dy;
        g_print("Shift: %d (%.2dx%.2d) (%.2f, %.2f)\n", d, Vw, Vh, dx, dy);
        g_print("Dragging: offset (%.1f, %.1f), new position (%.1f, %.1f)\n",
            drag_data->offset_x, drag_data->offset_y,
            drag_data->drag_start_x + drag_data->offset_x,
            drag_data->drag_start_y + drag_data->offset_y);
    }
    redraw();
}

void GuiApp::onDragEnd() {
    if (!key_data->is_ctrl) {
        int angleX = floor(1.5 * 90 / Cw * drag_data->offset_y);
        int angleY = floor(1.5 * 90 / Cw * drag_data->offset_x);
        tr.rotation_x = tr_copy.rotation_x + angleX;
        tr.rotation_y = tr_copy.rotation_y + angleY;
    } else {
        double dx = 1.0 * drag_data->offset_x * Vw / Cw / d;
        double dy = 1.0 * drag_data->offset_y * Vh / Ch / d;
        tr.trans_x = tr_copy.trans_x + dx;
        tr.trans_y = tr_copy.trans_y - dy;
    }
    shapeVert = s21::Transformer::Rotate(tr.rotation_x, tr.rotation_y, tr.rotation_z, shapeVert);
    tr.rotation_x = 0;
    tr.rotation_y = 0;
    tr.rotation_z = 0;
    redraw();
}

void GuiApp::onScroll(double dx, double dy) {
    // d = d - dy;
    // d = d > 0 ? d : 1;
    tr.scale += dy;
    redraw();
}

void GuiApp::onDraw(cairo_t* cr, int width, int height) {
    Cw = width;
    Ch = height;
    Vw = Cw/20;
    Vh = Ch/20;
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_paint(cr);
    cairo_set_line_width(cr, 1);
    cairo_translate(cr, 0, 0);
    cairo_scale(cr, 1, 1);
    DrawSomething(cr, this);
    std::cout << "[onDraw] " << std::format("w{},h{}\n", width, height);
}

void GuiApp::redraw() {
    gtk_widget_queue_draw(GTK_WIDGET(paper));
}

void GuiApp::on_drag_begin(GtkGestureDrag *gesture, double start_x, double start_y, gpointer user_data) {
    DragData *data = (DragData*)user_data;
    data->is_dragging = TRUE;
    data->drag_start_x = start_x;
    data->drag_start_y = start_y;
    data->offset_x = 0;
    data->offset_y = 0;
    g_print("Drag started at (%.1f, %.1f)\n", start_x, start_y);

    // Перерисовываем виджет
    GuiApp *self = static_cast<GuiApp*>(data->app);
    self->onDragBegin();
}

void GuiApp::on_drag_update(GtkGestureDrag *gesture, double offset_x, double offset_y, gpointer user_data) {
    DragData *data = (DragData*)user_data;

    if (!data->is_dragging) return;
    
    // Вычисляем новую позицию
    data->offset_x = offset_x;
    data->offset_y = offset_y;
    
    // Перерисовываем виджет
    GuiApp *self = static_cast<GuiApp*>(data->app);
    self->onDragUpdate();
}

void GuiApp::on_drag_end(GtkGestureDrag *gesture, double offset_x, double offset_y, gpointer user_data) {
    DragData *data = (DragData*)user_data;
    data->is_dragging = FALSE;
    g_print("Drag ended. Final offset (%.1f, %.1f)\n", offset_x, offset_y);

    // Перерисовываем виджет
    GuiApp *self = static_cast<GuiApp*>(data->app);
    self->onDragEnd();
}

gboolean GuiApp::on_scroll(GtkEventControllerScroll *controller, gdouble dx, gdouble dy, gpointer user_data) {
    if (dy > 0) {
        g_print("Scroll Down: %.0f\n", dy);
    } else if (dy < 0) {
        g_print("Scroll Up: %.0f\n", -dy);
    }    
    if (dx != 0) {
        g_print("Horizontal Scroll: %.0f\n", dx);
    }
    
    // Перерисовываем виджет
    GuiApp* self = static_cast<GuiApp*>(user_data);
    self->onScroll(dx, dy);
    
    return GDK_EVENT_STOP;
}

gboolean GuiApp::on_key_pressed(GtkEventControllerKey *controller, guint keyval, guint keycode, GdkModifierType state, gpointer user_data) {    
    KeyData *data = (KeyData*)user_data;

    g_print("state %d GDK_CONTROL_MASK %d\n", state, GDK_CONTROL_MASK);

    // Проверка Ctrl
    if (keyval == GDK_KEY_Control_L || keyval == GDK_KEY_Control_R) {
        g_print("Комбинация Ctrl нажата\n");
        data->is_ctrl = true;
        return GDK_EVENT_STOP;
    }    
    // Проверка Alt
    if (keyval == GDK_KEY_Alt_L || keyval == GDK_KEY_Alt_R) {
        g_print("Комбинация Alt нажата\n");
        data->is_alt = true;
        return GDK_EVENT_STOP;
    }
    
    return GDK_EVENT_PROPAGATE;
}

gboolean GuiApp::on_key_released(GtkEventControllerKey *controller, guint keyval, guint keycode, GdkModifierType state, gpointer user_data) {
    KeyData *data = (KeyData*)user_data;

    // Проверка Ctrl
    if (keyval == GDK_KEY_Control_L || keyval == GDK_KEY_Control_R) {
        g_print("Комбинация Ctrl отжата\n");
        data->is_ctrl = false;
        return GDK_EVENT_STOP;
    }    
    // Проверка Alt
    if (keyval == GDK_KEY_Alt_L || keyval == GDK_KEY_Alt_R) {
        g_print("Комбинация Alt отжата\n");
        data->is_alt = false;
        return GDK_EVENT_STOP;
    }

    return GDK_EVENT_PROPAGATE;
}
