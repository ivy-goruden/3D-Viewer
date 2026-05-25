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
    std::string settings;
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
    int vertSize;
  public:
    AppData(std::string sets) {
        settings = sets;
        angleX = 0;
        angleY = 0;
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
    int getVertSize() const {
        return vertSize;
    }
    void setPath(std::string val) {
        path = val;
        saveToFile(settings);
    }

    void setAngleX(int val) {
        angleX = val;
        saveToFile(settings);
    }

    void setAngleY(int val) {
        angleY = val;
        saveToFile(settings);
    }

    void setAngleZ(int val) {
        angleZ = val;
        saveToFile(settings);
    }

    void setShift(int val) {
        shift = val;
        saveToFile(settings);
    }

    void setShiftV(int val) {
        shiftV = val;
        saveToFile(settings);
    }

    void setZoom(int val) {
        zoom = val;
        saveToFile(settings);
    }

    void setScale(int val) {
        scale = val;
        saveToFile(settings);
    }

    void setLineSwitch(bool val) {
        lineSwitch = val;
        saveToFile(settings);
    }

    void setProjSwitch(bool val) {
        projSwitch = val;
        saveToFile(settings);
    }

    void setFillSwitch(bool val) {
        fillSwitch = val;
        saveToFile(settings);
    }

    void setVertMode(Shape mode) {
        vertMode = mode;
        saveToFile(settings);
    }

    void setColor(Rgb val) {
        color = val;
        saveToFile(settings);
    }
    
    void setBgColor(Rgb val) {
        bgcolor = val;
        saveToFile(settings);
    }

    void setWeight(int val) {
        weight = val;
        saveToFile(settings);
    }
    
    void setVertSize(int val) {
        vertSize = val;
        saveToFile(settings);
    }

    json toJson() const {        
        return json{
            { "path", getPath() },
            { "color", { getColor().red, getColor().green, getColor().blue, getColor().alpha } },
            { "bgcolor", { getBgColor().red, getBgColor().green, getBgColor().blue, getBgColor().alpha } },
            { "weight", getWeight() }
        };
    }
    
    void fromJson(const json& j) {
        std::vector<double> color_data = j.at("color").get<std::vector<double>>();
        std::vector<double> bgcolor_data = j.at("bgcolor").get<std::vector<double>>();

        setPath(j.at("path").get<std::string>());
        setColor(Rgb{color_data[0], color_data[1], color_data[2], color_data[3]});
        setBgColor(Rgb{bgcolor_data[0], bgcolor_data[1], bgcolor_data[2], bgcolor_data[3]});
        setWeight(j.at("weight").get<int>());
    }
    
    bool saveToFile(const std::string& filename) const {
        try {
            std::ofstream file(filename);
            if (!file.is_open()) return false;
            file << toJson().dump(4);
            return true;
        } catch (...) {
            std::cout << "error" << std::endl;
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
