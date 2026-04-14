#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>

namespace s21 {

    enum ObjKind {
        VERTEX,
        POINT,
        TEXTURE,
        NORMAL,
        FACE,
        LINE,
        POLIGON,
        OBJKIND_NUM
    };

    struct VertexObj_t {
        double x,y,z,w;
    };

    struct PointObj_t {
        double u,v,w;
    };

    struct TextureObj_t {
        double u,v,w;
    };

    struct NormalObj_t {
        double i,j,k;
    };

    struct FaceElementObj_t {
        int vi,ti,ni;
    };

    struct LineELementObj_t {
        int vi,ti;
    };

    typedef std::vector<FaceElementObj_t> FaceObj_t;

    typedef std::vector<LineELementObj_t> LineObj_t;

    typedef std::vector<int> PoligonObj_t;

    struct ParcedNumbers {
        std::string obj;
        std::vector<double> numbers;
    };

    struct ParcedWords {
        std::string obj;
        std::vector<std::string> words;
    };

    class ObjLoader {
        std::vector<VertexObj_t> vertices;
        std::vector<PointObj_t> points;
        std::vector<TextureObj_t> textures;
        std::vector<NormalObj_t> normals;
        std::vector<FaceObj_t> faces;
        std::vector<LineObj_t> lines;
        std::vector<PoligonObj_t> poligons;

        FILE* pObjFile;

      public:
        ObjLoader();
        ~ObjLoader();
        bool openFile(const char* filename);
        void closeFile();
        void loadObjFile(char* fileName);
        void readObj();
        void readVertex(char* line);
        void readTexture(char* line);
        void readPoint(char* line);
        void readNormal(char* line);
        void readFace(char* line);
        void readLine(char* line);
        void readPoligon(char* line);
      private:
        ObjKind parseObjKind(char* line);
        ParcedNumbers parseNumbers(char* line);
        ParcedWords parseWords(char* line);
        std::vector<int> parseDelimitedIndexes(std::string& element, char delimiter);
    };

}
