#include "../include/camera/camera.hpp"
#include "../include/figure/figure.hpp"
#include "../include/globals.h"
#include "../include/grid.hpp"
namespace s21 {

class Controller {
   protected:
    Figure* figure_ = nullptr;
    bool parallel_projection_ = false;
    float angleX_;
    float angleY_;
    float angleZ_;
    Camera* camera_;
    float scale_;
    float shiftx_;
    float shifty_;
    float diagonal_;
    Poly_Proj_t getFigureProjection(const matrix_t original);
    Poly_Proj_t projection_;

    int prev_angleX_;
    int prev_angleY_;
    int prev_angleZ_;

    float prev_shiftX_;
    float prev_shiftY_;

   public:
    Controller();
    ~Controller();
    void loadFigure(const char* filename, int, int);
    //Poly_Proj_t getFigure();
    void toggleProjection();
    void shiftX(int);
    void shiftY(int);
    matrix_t getMatrix();
    Edge_t getEdges();
    std::vector<s21::FaceObj_t> getPolygons();
    std::vector<TextureObj_t> getTextures();
    float getAngleX();
    float getAngleY();
    float getAngleZ();
    void setAngleX(int angle);
    void setAngleY(int angle);
    void setAngleZ(int angle);
    void setScale(float scale);
    void setCameraZ(float);
    float getScale();
    float getCameraZ();
    int getEdgesNum();
    int getVerticesNum();
    Bounds getFigureBounds();
    void FigureView_Sett(int canvasW, int canvasH);
    matrix_t getMVP(float aspect);
};
}  // namespace s21