namespace s21{
    
    //Класс афинных преобразований фигур
    class Transformer{
        public:
            Transformer();
            matrix_t static Rotate(int angleX, int angleY, int angleZ, matrix_t);
            Vert_t static getFigureProjection(matrix_t);  

    }
}