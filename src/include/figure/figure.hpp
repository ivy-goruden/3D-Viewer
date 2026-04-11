namespace s21{

class Figure(){
    protected:
        Vert_t vertices_;
        Poly_t polygons_;
        Node_t nodes_;
        matrix_t matrix_;

    
    public:
        Figure(Vert_t v, Poly_t p): vertices_(v), polygons_(p);
        Vert_t getVertices();
        Poly_t getPolygons();
        Node_t getNodes();
        matrix_t getMatrix();
        int getVerticesNum();
        int getNodesNum();
    private:
        void static genMatrix();
}

}
