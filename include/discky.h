#pragma once

#include "types.h"
#include "window.h"
#include <vector>
#include <string>
#include <functional>

using objId = size_t;
inline constexpr objId NO_PARENT = (objId)-1;

enum class objType{
    TRIANGLE,
    RECTANGLE,
    CIRCLE,
    LINE,
    POLY,
};

enum ERRORS{
    OUT_OF_MEM,
    TERMINAL_INFO,
    INVALID_INPUT,
    INTERNAL,
};

enum unitType{
    COOR_PIX,
    COOR_NOR,
    LEN_PIX,
    LEN_NOR,
    TXT_COORD_PIX,
    TXT_COORD_NOR,
};
enum class antiAliasing{
    AA_NONE,
    AA_LOW, // 2X2 = 4
    AA_MEDIUM, // 4X4 = 16
    AA_HIGH, // 8X8 = 64
};

struct Coordinate{
    unitType type;
    int x;
    int y;
    double norX;
    double norY;
};
inline Coordinate COORD_NOR(double a, double b){return Coordinate{unitType::COOR_NOR , -1 , -1 , a , b};}
inline Coordinate COORD_PIX(int a , int b){return Coordinate{unitType::COOR_PIX ,a , b , -1 , -1};}
inline Coordinate LEN_MONO_PIX(int x){return Coordinate{unitType::LEN_PIX , x , -1 , -1 , -1};}
inline Coordinate LEN_MONO_NOR(double x){return Coordinate{unitType::LEN_NOR , -1 , -1 , x , -1};}
inline Coordinate LEN_DI_PIX(int a , int b){return Coordinate{unitType::LEN_PIX , a , b , -1 , -1};}
inline Coordinate LEN_DI_NOR(double a , double b){return Coordinate{unitType::LEN_NOR , -1 , -1 , a , b};}
inline Coordinate COORD_TXT_PIX(int a , int b){return Coordinate{unitType::TXT_COORD_PIX , a , b/2 , -1 , -1};}
inline Coordinate COORD_TXT_NOR(double a , double b){return Coordinate{unitType::TXT_COORD_NOR , -1 , -1 , a , b};}


struct objColor{
    int r;
    int g;
    int b;
};

inline objColor DISCKY_COLOR_RGB(int r , int g , int b){return objColor{r , g , b};}

struct objects{
    std::vector<Coordinate> vertex;
    objType type;
    objColor color;
    bool removed=0;
    double border=1;
    double opacity=1.0;
    int minX;
    int maxX;
    int minY;
    int maxY;
    void removeObj();
    objId parent = NO_PARENT;
    std::vector<objId> children;
    bool clipChildren = 1;

};

struct rawText{
    std::string txt;
    Coordinate vertex;
    objColor color ;
};
struct Screen{
    int x=0;
    int y=0;
    std::vector<objColor> pixels;
};
class Discky{
    public:
        TerminalInfo terminalInfo;
        std::vector<objects> objs;
        std::vector<rawText> rawTxts;
        Screen backBuffer;
        Screen frontBuffer;
        objColor bgColor;
        std::function<void(ERRORS , const std::string&)> handleErrors;
        bool forceRedraw=1;
        bool permanantForceRedraw=0;
        antiAliasing aaMode = antiAliasing::AA_NONE;
        Discky();
        void callErrorHandle(ERRORS error , const std::string& msg) const;
        void setErrorHandleFunc(std::function<void(ERRORS , const std::string&)> handler);
        void setBackground(const objColor& color);
        objId drawRectangle(const Coordinate& a, const Coordinate& b ,const objColor& color , const double& border =1.0 , const double& opacity =1.0);
        objId drawCircle(const Coordinate& a , const Coordinate& b , const objColor& color , const double& border = 1.0 , const double& opacity = 1.0);
        objId drawTriangle(const Coordinate& a , const Coordinate& b , const Coordinate& c , const objColor& color , const double& border = 1.0 , const double& opacity = 1.0);
        objId drawLine(const Coordinate& a , const Coordinate& b, const objColor& color , const double& opacity= 1.0);
        objId drawPoly(const std::vector<Coordinate> &ver ,const objColor &color , const double& border =1.0 , const double& opacity = 1.0);
        objId drawRawTxt(const Coordinate& a ,const std::string& txt , const objColor& color);
        void removeObj(objects& obj);
        void render();
        void display();
        void refresh();
        int getTerminalSizeX();
        int getTerminalSizeY();
        void setAntiAliasing(const antiAliasing& mode);
        bool isObjectTouchingTop(objId obj);
        bool isObjectTouchingBottom(objId obj);
        bool isObjectTouchingLeft(objId obj);
        bool isObjectTouchingRight(objId obj);
        bool isTextTouchingTop(objId obj);
        bool isTextTouchingBottom(objId obj);
        bool isTextTouchingLeft(objId obj);
        bool isTextTouchingRight(objId obj);

        std::vector<int> objRecycleBin;
        bool checkOverlap(const objId ia, const objId ib);
        void addChild(objId parent , objId child);
        void setForceRedraw();
        void clearForceRedraw();
};

void iniTerminal();
void endDiscky();

#include "colors.h"
#include "render.h"