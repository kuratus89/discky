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
};
struct objects{
    std::vector<Coordinate> vertex;
    objType type;
    objColor color;
    bool removed=0;
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
        Discky();
        void callErrorHandle(ERRORS error , const std::string& msg) const;
        void setErrorHandleFunc(std::function<void(ERRORS , const std::string&)> handler);
        void setBackground(const objColor& color);
        objects& drawRectangle(const Coordinate& a, const Coordinate& b ,const objColor& color);
        objects& drawCircle(const Coordinate& a , const int radiusPixels , const objColor& color);
        objects& drawTriangle(const Coordinate& a , const Coordinate& b , const Coordinate& c , const objColor& color);
        objects& drawLine(const Coordinate& a , const Coordinate& b, const objColor& color);
        objects& drawPoly(const std::vector<Coordinate> &ver ,const objColor &color);
        void render();
        void display();
        void refresh();
        int getTerminalSizeX();
        int getTerminalSizeY();
        std::vector<int> objRecycleBin;
        
};
void iniTerminal();
#include "colors.h"
#include "render.h"