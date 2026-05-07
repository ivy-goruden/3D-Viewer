#ifndef GRID_HPP
#define GRID_HPP


class Grid {
    private:
    matrix_t m;

    public:
    Grid();
    ~Grid();
    createGrid(s21::Figure& figure);
    createGrid(Bounds bounds);
    createGrid(double x1, double x2, double y1, double y2, double z1, double z2, double step);
};

Grid::Grid() {
    m = matrix_t();
}

Grid::~Grid() {
    //
}

Grid::createGrid() {

}

#endif
