#pragma once

#include "types.h"
#include "window.h"
#include <vector>
#include <string>
#include <functional>
enum class objType{
    TRIANGLE,
    RECTANGLE,
    CIRCLE,
    LINE,
    POLY,
    IMAGE,
    RAW_TEXT,
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
inline Coordinate NOR_COORD(double a, double b){return Coordinate{unitType::COOR_NOR , -1 , -1 , a , b};}
inline Coordinate SCR_COORD(int a , int b){return Coordinate{unitType::COOR_PIX ,a , b , -1 , -1};}
inline Coordinate SCR_LENGTH(int a){return Coordinate{unitType::LEN_PIX , a , -1, -1 , -1 };}
struct objColor{
    int r;
    int g;
    int b;
    int o;
};
struct objects{
    std::vector<Coordinate> vertex;
    objType type;
    objColor color;
    bool removed=0;
    double boder=1;
    double opacity=1.0;
    void removeObj();
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
        Screen backBuffer;
        Screen frontBuffer;
        objColor bgColor;
        std::function<void(ERRORS , const std::string&)> handleErrors;
        bool forceRedraw=1;
        antiAliasing aaMode = antiAliasing::AA_NONE;
        Discky();
        void callErrorHandle(ERRORS error , const std::string& msg) const;
        void setErrorHandleFunc(std::function<void(ERRORS , const std::string&)> handler);
        void setBackground(const objColor& color);
        objects& drawRectangle(const Coordinate& a, const Coordinate& b ,const objColor& color , const double& boder =1.0 , const double& opacity =1.0);
        objects& drawCircle(const Coordinate& a , const int radiusPixels , const objColor& color , const double& boder = 1.0 , const double& opacity = 1.0);
        objects& drawTriangle(const Coordinate& a , const Coordinate& b , const Coordinate& c , const objColor& color , const double& boder = 1.0 , const double& opacity = 1.0);
        objects& drawLine(const Coordinate& a , const Coordinate& b, const objColor& color , const double& opacity= 1.0);
        objects& drawPoly(const std::vector<Coordinate> &ver ,const objColor &color , const double& boder =1.0 , const double& opacity = 1.0);
        void removeObj(objects& obj);
        void render();
        void display();
        void refresh();
        int getTerminalSizeX();
        int getTerminalSizeY();
        void setAntiAliasing(const antiAliasing& mode);
        std::vector<int> objRecycleBin;
        
};
void iniTerminal();
#include "colors.h"
#include "render.h"