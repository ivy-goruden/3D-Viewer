#ifndef APP_DATA
#define APP_DATA

#include <format>
#include <string>
#include "rgb.hpp"
#include "../simple_canvas.hpp"

class GuiApp;

struct AppData {
  private:
    std::string path;
    int angleX;
    int angleY;
    int angleZ;
    int shift;
    int zoom;
    bool lineSwitch;
    bool projSwitch;
    bool fillSwitch;
    Shape vertMode;
    Rgb color;
    Rgb bgcolor;
    int weight;
  public:
    AppData() {
      angleX = 0;
      angleY = 0;
      angleZ = 0;
      shift = 0;
      zoom = 0;
      lineSwitch = false;
      projSwitch = false;
      fillSwitch = false;
      vertMode = None;
      color = Rgb{0,0,0,1};
      bgcolor = Rgb{1,1,1,1};
      weight = 1;
    }
    std::string getPath() const {
        return path;
    }
    int getAngleX() const {
        return angleX;
    }
    int getAngleY() const {
        return angleY;
    }
    int getAngleZ() const {
        return angleZ;
    }
    int getShift() const {
        return shift;
    }
    int getZoom() const {
        return zoom;
    }
    bool getLineSwitch() const {
        return lineSwitch;
    }
    bool getProjSwitch() const {
        return projSwitch;
    }
    bool getFillSwitch() const {
        return fillSwitch;
    }
    Shape getVertMode() const {
        return vertMode;
    }
    Rgb getColor() const {
        return color;
    }
    Rgb getBgColor() const {
        return bgcolor;
    }
    int getWeight() const {
        return weight;
    }
    void setPath(std::string val) {
        path = val;
    }
    void setAngleX(int val) {
        angleX = val;
    }
    void setAngleY(int val) {
        angleY = val;
    }
    void setAngleZ(int val) {
        angleZ = val;
    }
    void setShift(int val) {
        shift = val;
    }
    void setZoom(int val) {
        zoom = val;
    }
    void setLineSwitch(bool val) {
        lineSwitch = val;
    }
    void setProjSwitch(bool val) {
        projSwitch = val;
    }
    void setFillSwitch(bool val) {
        fillSwitch = val;
    }
    void setVertMode(Shape mode) {
        vertMode = mode;
    }
    void setColor(Rgb val) {
        color = val;
    }
    void setBgColor(Rgb val) {
        bgcolor = val;
    }
    void setWeight(int val) {
        weight = val;
    }
    std::string toString() {
        std::string sPath = path == "" ? "не выбран" : path;
        return std::format("Файл: {} | Толщина линии: {}", sPath, weight);
    }
};

#endif
