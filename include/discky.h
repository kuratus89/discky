#pragma once
#include "window.h"
#include "memory.h"
#include "colors.h"
#include "render.h"
#include <locale.h>

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

typedef struct{
    int x;
    int y;
    Vec pixels; // vector of objColors
}Screen;



typedef struct{
    TerminalInfo terminalInfo;
    Vec objs; // vector of objects
    Screen* backBuffer;
    Screen* frontBuffer;
    objColor bgColor;
    Vec garbge;
    void (*handleErrors)(ERRORS error , char* msg);
    
}Discky;

void callErrorHandle(const Discky* discky , const ERRORS error , const char* msg);
void renderDiscky(Discky* discky);