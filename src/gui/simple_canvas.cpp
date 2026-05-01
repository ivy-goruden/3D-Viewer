#include "simple_canvas.hpp"

// Реализация методов класса SimpleCanvas для GTK4

SimpleCanvas::SimpleCanvas(GtkWidget* drawing_area) {

    // Устанавливаем функцию рисования (вместо сигнала "draw")
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(drawing_area),
                                   on_draw_static,
                                   this, nullptr);
    scale_ = 10;
    xStart_ = 0;
    yStart_ = 0;
    widget_ = drawing_area;
    c_ = new s21::Controller();
    dotColor_ = Rgb(1,0,0,1);
    lineColor_ = Rgb(0,1,0,1);
    polyColor_ = Rgb(0,0,1,1);
    bgColor_ = Rgb(1,1,1,1);
    fillPoly_ = false;
    lineWidth_ = 0.01;
    vertWidth_ = 0.1;
    vertType_ = None;
    lineType_ = Solid;
}

SimpleCanvas::~SimpleCanvas() {
    // Деструктор (можно оставить пустым или освободить ресурсы)
}

void SimpleCanvas::draw_line(cairo_t* cr, double x1, double y1, double x2, double y2) {
    cairo_set_source_rgb(cr, lineColor_.red, lineColor_.green, lineColor_.blue);
    cairo_move_to(cr, x1, y1);
    cairo_line_to(cr, x2, y2);
    if (lineType_ == Dotted){
        double dashes[] = {lineWidth_*2, lineWidth_};
        int num_dashes = 2;
        double offset = 0.0;
        cairo_set_dash(cr, dashes, num_dashes, offset);
    }

    cairo_stroke(cr);
}

void SimpleCanvas::draw_polygon(cairo_t* cr, const std::vector<s21::Point>& points) {
    if (points.size() < 3) return;

    cairo_set_source_rgb(cr, polyColor_.red, polyColor_.green, polyColor_.blue);

    cairo_move_to(cr, points[0].x, points[0].y);
    for (size_t i = 1; i < points.size(); ++i) {
        cairo_line_to(cr, points[i].x, points[i].y);
    }
    cairo_close_path(cr);
    if (fillPoly_){
        cairo_fill(cr);
    }else{
        cairo_stroke(cr);
    }

}

void SimpleCanvas::draw_dot(cairo_t* cr, double x, double y) {
    switch(vertType_){
        case Circle:
            cairo_arc(cr, x, y, lineWidth_*2, 0, 2 * M_PI);
        case Rect:
            double d = lineWidth_*2*std::sqrt(2)/2;
            cairo_rectangle (cr, x-d, y-d, 2*d, 2*d);
    }
    cairo_set_source_rgb(cr, dotColor_.red, dotColor_.green, dotColor_.blue);
    cairo_fill(cr);
}

void SimpleCanvas::drawVert(cairo_t* cr){
    for (const auto& point : projection_) {
        draw_dot(cr, point.x, point.y);
    }
}

void SimpleCanvas::drawEdges(cairo_t* cr){
    s21::Edge_t edges = c_->getEdges();
    for (s21::Seg_t s: edges){
        draw_line(cr, projection_[s.start].x, projection_[s.start].y, projection_[s.end].x, projection_[s.end].y);

    }
}

void SimpleCanvas::drawFaces(cairo_t* cr){
    s21::Poly_t poly = c_->getPolygons();
    for(auto dots: poly){
        std::vector<s21::Point> points;
        for(int n: dots){
            points.push_back(projection_[n]);
        }
        draw_polygon(cr, points);
    }
}

void SimpleCanvas::on_draw_static(GtkDrawingArea* area, cairo_t* cr, int width, int height, gpointer user_data) {
    SimpleCanvas* self = static_cast<SimpleCanvas*>(user_data);
    self->onDraw(cr, width, height);
}

void SimpleCanvas::loadFigure(const char* filename) {
    projection_ = c_->loadFigure(filename);
}

void SimpleCanvas::redraw(){
    gtk_widget_queue_draw(GTK_WIDGET(widget_));
}

void SimpleCanvas::onDraw(cairo_t* cr, int width, int height) {
    cairo_set_source_rgb(cr, bgColor_.red, bgColor_.green, bgColor_.blue);
    cairo_paint(cr);
    cairo_set_line_width(cr, lineWidth_);
    cairo_translate(cr, width/2+xStart_, height/2+yStart_);
    cairo_scale(cr, scale_, scale_);

    drawFaces(cr);
    drawEdges(cr);
    drawVert(cr);

}

void SimpleCanvas::setScale(double scale){
    scale_ = scale;
}

void SimpleCanvas::setPosX(double x){
    xStart_ = x;
}

void SimpleCanvas::setPosY(double y){
    yStart_ = y;
}

void SimpleCanvas::rotateAbs(double x, double y, double z){
    c_->setAngleX(x);
    c_->setAngleY(y);
    c_->setAngleZ(z);
    projection_ = c_->getFigure();
}

void SimpleCanvas::rotateX(double x){
    c_->setAngleX(x);
    projection_ = c_->getFigure();
}

void SimpleCanvas::rotateY(double y){
    c_->setAngleY(y);
    projection_ = c_->getFigure();
}

void SimpleCanvas::rotateZ(double z){
    c_->setAngleY(z);
    projection_ = c_->getFigure();
}

void SimpleCanvas::toggleProjection(){
    c_->toggleProjection();
    projection_ = c_->getFigure();
}

void SimpleCanvas::togglePolyFill(){
    fillPoly_ = !fillPoly_;
}

void SimpleCanvas::setVertType(int type){
    switch(type){
        case 1:
            vertType_ = Shape(Circle);
            break;
        case 2:
            vertType_ = Shape(Rect);
            break;
        default:
            vertType_ = Shape(None);
            break;
    }
}

void SimpleCanvas::setLineType(int type){
    switch(type){
        case 1:
            lineType_ = Stroke(Dotted);
            break;
        default:
            lineType_ = Stroke(Solid);
            break;
    }
}

void SimpleCanvas::setBgColor(Rgb color){
    bgColor_ = color;
}
void SimpleCanvas::setVertColor(Rgb color){
    dotColor_ = color;
}

void SimpleCanvas::setLineWidth(double width){
    lineWidth_ = width;
}

void SimpleCanvas::setZoom(double zoom){
    c_->setScale(zoom);
    projection_ = c_->getFigure();
}

double SimpleCanvas::getAngleX() {
    return c_->getAngleX();
}

double SimpleCanvas::getAngleY() {
    return c_->getAngleY();
}

double SimpleCanvas::getAngleZ() {
    return c_->getAngleZ();
}

int SimpleCanvas::getEdgesNum(){
    return c_->getEdgesNum();
}
int SimpleCanvas::getVerticesNum(){
    return c_->getVerticesNum();
}
