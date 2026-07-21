#pragma once

#include "types.h"
#include "window.h"
#include "memory.h"
#include "screen.h"
#include <locale.h>

#define NOR_COORD(A,B) (coordinate){COORD_NORMALIZED , -1 , -1 , A , B}
#define SCR_COORD(A,B) (coordinate){COORD_TERMINAL_SCREEN , A, B , -1 , -1}

typedef enum{
    OBJ_TRIANGLE,
    OBJ_RECTANGLE,
    OBJ_CIRCLE,
    OBJ_LINE,
    OBJ_IMAGE,
    OBJ_RAW_TEXT,
}objType;

typedef enum{
    ERROR_OUT_OF_MEM,
    ERROR_TERMINAL_INFO,

    ERROR_INTERNAL,
}ERRORS;

typedef enum{
    COORD_TERMINAL_SCREEN,
    COORD_NORMALIZED
}coordinateType;

typedef struct {
    coordinateType type;
    int x;
    int y;
    double norX;
    double norY;
}coordinate;

typedef struct{
    int r;
    int g;
    int b;
}objColor;

typedef struct{
    Vec vertex;// vector of coordinate
    objType type;
    objColor color;
}objects;

struct Screen{
    int x;
    int y;
    Vec pixels; // vector of objColors
};



struct Discky{
    TerminalInfo terminalInfo;
    Vec objs; // vector of objects
    Screen* backBuffer;
    Screen* frontBuffer;
    objColor bgColor;
    Vec garbge;
    void (*handleErrors)(ERRORS error , char* msg);
    
};

void callErrorHandle(const Discky* discky , const ERRORS error , char* msg);
void renderDiscky(Discky* discky);
void clearGarbge(Discky* discky);
void displayDiscky(Discky* discky);
void iniDiscky(Discky* discky);
void refreshDiscky(Discky* discky);
void endDiscky(Discky* discky);
objects* disckyDrawRec(Discky* discky , const coordinate x , const coordinate y , const objColor color);
void displayDiscky(Discky* discky);
void setErrorHandleFunc(Discky* discky , void (*handleError)(ERRORS error , char* msg));
void setDisckyBackground(Discky* discky , const objColor color);


#include "colors.h"
#include "render.h"