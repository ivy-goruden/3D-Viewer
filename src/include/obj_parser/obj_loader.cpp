#include "obj_loader.hpp"

#include <stdexcept>
#include <vector>
#include <sstream>
#include <cstring>
#include <string>
#include <format>

#define MAX_LINE_SIZE 1000

namespace s21 { 

    ObjLoader::ObjLoader() {
        pObjFile = NULL;
    }

    ObjLoader::~ObjLoader() {
        closeFile();
    }

    ObjKind ObjLoader::parseObjKind(char* line) {
        ObjKind kind = OBJKIND_NUM;
        
        size_t s = 0;
        while (line[s] == 32 && line[s] != '\0') {
            s++;
        }
        size_t e = s;
        while (line[e] != 32 && line[e] != '\0') {
            e++;
        }

        size_t len = e - s;
        char sKind[10] = {0};
        strncpy(sKind, line + s, len);

        if (strcmp("v", sKind) == 0) kind = VERTEX;
        else if (strcmp("vp", sKind) == 0) kind = POINT;
        else if (strcmp("vt", sKind) == 0) kind = TEXTURE;
        else if (strcmp("vn", sKind) == 0) kind = NORMAL;
        else if (strcmp("f", sKind) == 0) kind = FACE;
        else if (strcmp("l", sKind) == 0) kind = LINE;
        else if (strcmp("p", sKind) == 0) kind = POLIGON;

        return kind;
    }

    std::vector<int> ObjLoader::parseDelimitedIndexes(std::string& element, char delimiter) {
        std::vector<int> result;
        int current = 0;
        bool isNumber = false;

        for (char ch : element) {
            if (ch == delimiter) {
                result.push_back(current);
                isNumber = false;
                current = 0;
            } else if (ch >= '0' && ch <= '9') {
                current = current * 10 + (ch - '0');
                isNumber = true;
            }
        }
        if (isNumber) {
            result.push_back(current);
        }
        return result;
    }

    bool ObjLoader::openFile(const char* filename) {
        pObjFile = fopen(filename, "r");
        if (!pObjFile) {
            std::string message = std::format("Failed to open file: %s", filename);
            throw std::invalid_argument(message);
        }
        return true;
    }

    void ObjLoader::closeFile() {
        if (pObjFile) {
            fclose(pObjFile);
            pObjFile = nullptr;
        }
    }

    void ObjLoader::loadObjFile(const char* fileName) {
        openFile(fileName);
        readObj();
        closeFile();
    }

    void ObjLoader::readVertex(char* line) {
        ParcedNumbers data = parse<double>(line);
        if (data.items.size() < 3) {
            throw std::invalid_argument("Failed to read vertex");
        }

        VertexObj_t obj;
        obj.x = data.items[0];
        obj.y = data.items[1];
        obj.z = data.items[2];
        obj.hasWeight = false;
        if (data.items.size() > 3) {
            obj.w = data.items[3];
            obj.hasWeight = true;
        }
        vertices.push_back(obj);
    }
    
    void ObjLoader::readTexture(char* line) {
        ParcedNumbers data = parse<double>(line);
        if (data.items.size() < 2) {
            throw std::invalid_argument("Failed to read texture");
        }

        TextureObj_t obj;
        obj.u = data.items[0];
        obj.v = data.items[1];
        obj.hasWeight = false;
        if (data.items.size() > 2) {
            obj.w = data.items[2];
            obj.hasWeight = true;
        }
        textures.push_back(obj);
    }
    
    void ObjLoader::readPoint(char* line) {
        ParcedNumbers data = parse<double>(line);
        if (data.items.size() == 0) {
            throw std::invalid_argument("Failed to read point");
        }

        PointObj_t obj;
        obj.u = data.items[0];
        obj.hasV = false;
        obj.hasW = false;
        if (data.items.size() > 1) {
            obj.v = data.items[1];
            obj.hasV = true;
        }  
        if (data.items.size() > 2) {
            obj.w = data.items[2];
            obj.hasW = true;
        }
        points.push_back(obj);
    }
    
    void ObjLoader::readNormal(char* line) {
        ParcedNumbers data = parse<double>(line);
        if (data.items.size() < 3) {
            throw std::invalid_argument("Failed to read normal");
        }

        NormalObj_t obj;
        obj.i = data.items[0];
        obj.j = data.items[1];
        obj.k = data.items[2];        
        normals.push_back(obj);
    }
    
    void ObjLoader::readFace(char* line) {
        ParcedWords data = parse<std::string>(line);
        if (!data.items.size()) {
            throw std::invalid_argument("Failed to read face");
        }
        FaceObj_t f;
        for (size_t i = 0; i < data.items.size(); i++) {
            std::vector indexes = parseDelimitedIndexes(data.items[i], '/');
            FaceElementObj_t fe;
            fe.vi = indexes.size() > 0 ? indexes[0] : 0;
            fe.ti = indexes.size() > 1 ? indexes[1] : 0;
            fe.ni = indexes.size() > 2 ? indexes[2] : 0;
            if (!fe.vi) {
                throw std::invalid_argument("Failed to read face");
            }
            f.push_back(fe);
        }
        if (f.size()) {
            faces.push_back(f);
        }
    }

    void ObjLoader::readLine(char* line) {
        ParcedWords data = parse<std::string>(line);
        if (!data.items.size()) {
            throw std::invalid_argument("Failed to read line");
        }
        LineObj_t l;
        for (size_t i = 0; i < data.items.size(); i++) {
            std::vector<int> indexes = parseDelimitedIndexes(data.items[i], '/');
            LineELementObj_t le;
            le.vi = indexes.size() > 0 ? indexes[0] : 0;
            le.ti = indexes.size() > 1 ? indexes[1] : 0;
            if (!le.vi) {
                throw std::invalid_argument("Failed to read line");
            }
            l.push_back(le);
        }
        if (l.size()) {
            lines.push_back(l);
        }
    }

    void ObjLoader::readPoligon(char* line) {
        ParcedNumbers data = parse<double>(line);
        if (data.items.size() == 0) {
            throw std::invalid_argument("Failed to read poligon");
        }

        PoligonObj_t p;
        for (size_t i = 0; i < data.items.size(); i++) {
            p.push_back(data.items[i]);
        }
        if (data.items.size()) {            
            poligons.push_back(p);
        }
    }

    void ObjLoader::readObj() {
        if (!pObjFile) {
            throw std::invalid_argument("File not opened");
        }

        char line[MAX_LINE_SIZE];
        int lineNumber = 0;

        while (fgets(line, sizeof(line), pObjFile)) {
            lineNumber++;
            
            size_t len = strlen(line);
            if (len > 0 && line[len - 1] == '\n') {
                line[len - 1] = '\0';
                len--;
            }
            if (len == 0) continue;

            int kindObj = parseObjKind(line);
            printf("Line %d: %d - %s\n", lineNumber, kindObj, line);

            switch (kindObj) {
                case VERTEX:
                    readVertex(line);
                    break;
                case POINT:
                    readPoint(line);
                    break;
                case TEXTURE:
                    readTexture(line);
                    break;
                case NORMAL:
                    readNormal(line);
                    break;
                case FACE:
                    readFace(line);
                    break;
                case LINE:
                    readLine(line);
                    break;
                case POLIGON:
                    readPoligon(line);
                    break;
            }
        }
        if (ferror(pObjFile)) {
            throw std::invalid_argument("Read error occurred");
        }
    }

}
