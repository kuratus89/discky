#pragma once

#include "types.h"
#include "window.h"
// #include "memory.h"
#include <vector>
#include <string>
#include <functional>
// #include "screen.h"
// #include <locale.h>

// #define NOR_COORD(A,B) (coordinate){COORD_NORMALIZED , -1 , -1 , A , B}
// #define SCR_COORD(A,B) (coordinate){COORD_TERMINAL_SCREEN , A, B , -1 , -1}

enum class objType{
    TRIANGLE,
    RECTANGLE,
    CIRCLE,
    LINE,
    IMAGE,
    RAW_TEXT,
};

enum ERRORS{
    OUT_OF_MEM,
    TERMINAL_INFO,

    INTERNAL,
};

enum CoordinateType{
    TERMINAL_SCREEN,
    NORMALIZED
};

struct Coordinate{
    CoordinateType type;
    int x;
    int y;
    double norX;
    double norY;
};

inline Coordinate NOR_COORD(double a, double b){return Coordinate{CoordinateType::NORMALIZED , -1 , -1 , a , b};}
inline Coordinate SCR_COORD(int a , int b){return Coordinate{CoordinateType::TERMINAL_SCREEN ,a , b , -1 , -1};}
struct objColor{
    int r;
    int g;
    int b;
};

struct objects{
    std::vector<Coordinate> vertex;
    objType type;
    objColor color;
};

struct Screen{
    int x=0;
    int y=0;
    std::vector<objColor> pixels;
};



// struct Discky{
//     TerminalInfo terminalInfo;
//     Vec objs; // vector of objects
//     Screen* backBuffer;
//     Screen* frontBuffer;
//     objColor bgColor;
//     Vec garbge;
//     void (*handleErrors)(ERRORS error , char* msg);
    
// };

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
        objects& drawRec(const Coordinate& a , const Coordinate& b , const objColor& color);
        void render();
        void display();
        void refresh();
        
};

void iniTerminal();

// void callErrorHandle(const Discky* discky , const ERRORS error , char* msg);
// void renderDiscky(Discky* discky);
// void clearGarbge(Discky* discky);
// void displayDiscky(Discky* discky);
// void iniDiscky(Discky* discky);
// void refreshDiscky(Discky* discky);
// void endDiscky(Discky* discky);
// objects* disckyDrawRec(Discky* discky , const coordinate x , const coordinate y , const objColor color);
// void displayDiscky(Discky* discky);
// void setErrorHandleFunc(Discky* discky , void (*handleError)(ERRORS error , char* msg));
// void setDisckyBackground(Discky* discky , const objColor color);


#include "colors.h"
#include "render.h"