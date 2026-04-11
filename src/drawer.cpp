
#include "drawer.hpp"

namespace s21{
    Drawer::draw(Vert_t coords){
        for (auto y = coords.begin(); i!=coords.end(); i++){
            for (auto x = y.begin(); i!=y.end(); i++){

            }
            mvprintw(y, x, "%d", num);
        }
        refresh();
        getch();
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


    

    
