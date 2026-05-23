#ifndef APP_DATA
#define APP_DATA

#include <format>
#include <string>
#include <iostream>
#include <fstream>
#include "gui_globals.h"
#include "simple_canvas.hpp"

#include "third_party/nlohmann/json.hpp"

using json = nlohmann::json;

class GuiApp;

struct AppData {
  private:
    std::string path;
    int angleX;
    int angleY;
    int angleZ;
    int shift;
    int shiftV;
    int zoom;
    int scale;
    bool lineSwitch;
    bool projSwitch;
    bool fillSwitch;
    Shape vertMode;
    Rgb color;
    Rgb bgcolor;
    int weight;

  public:
    AppData() {
      angleX = 135;
      angleY = 90;
      angleZ = 0;
      shift = 0;
      shiftV = 0;
      zoom = 10;
      scale = 10;
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

    int getShiftV() const {
        return shiftV;
    }

    int getZoom() const {
        return zoom;
    }

    int getScale() const {
        return scale;
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

    void setShiftV(int val) {
        shiftV = val;
    }

    void setZoom(int val) {
        zoom = val;
    }

    void setScale(int val) {
        scale = val;
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

    json toJson() const {
        // return json{
        //     {"name", name},
        //     {"value", value},
        //     {"coefficient", coefficient},
        //     {"data", data}
        // };
    }
    
    void fromJson(const json& j) {
        // name = j.at("name").get<std::string>();
        // value = j.at("value").get<int>();
        // coefficient = j.at("coefficient").get<double>();
        // data = j.at("data").get<std::vector<int>>();
    }
    
    bool saveToFile(const std::string& filename) const {
        try {
            std::ofstream file(filename);
            if (!file.is_open()) return false;
            file << toJson().dump(4);
            return true;
        } catch (...) {
            return false;
        }
    }
    
    bool loadFromFile(const std::string& filename) {
        try {
            std::ifstream file(filename);
            if (!file.is_open()) return false;
            json j;
            file >> j;
            fromJson(j);
            return true;
        } catch (...) {
            return false;
        }
    }
    
    void printJson() const {
        std::cout << toJson().dump(4) << std::endl;
    }
};

#endif
