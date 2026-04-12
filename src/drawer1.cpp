
#include "drawer.hpp"
namespace s21{
    Drawer::draw(ProjectionData_t *projection){
        Vert_t* v = projection->vertices;
        Poly_t *p = projection->polygons;
        Node_t *n = projection->nodes;
        drawVert(v);
        drawNodes(v, n);
        drawPoly(v, p);
        refresh();
        getch();
    }
    Drawer::drawVert(Vert_t *v){
        for (auto i = v.begin(); i!=v.end(); i++){
            double y = i.second*scale_ +yStart_;
            double x = i.first*scale_ +xStart_;

            mvprintw(y, x, "%d", num);
        }
    }
    Drawer::drawPoly(Vert_t *v, Poly_t *p){
        
    }
    Drawer::drawNodes(Vert_t *v, Node_t *n){

    }
    Drawer::Drawer(){
        // Инициализация ncurses
        initscr();
        cbreak();
        noecho();
        X_START = 0
        Y_START = 0
        scale = SCALE
        // Включаем цвета
        start_color();
        init_pair(1, COLOR_GREEN, COLOR_BLACK);
        attron(COLOR_PAIR(1));
    }
    Drawer::~Drawer(){
        endwin();
    }
}


    

    
