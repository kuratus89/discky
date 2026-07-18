#pragma once
#include "window.h"
#include "memory.h"
#include "colors.h"

typedef enum{
    OBJ_TRIANGLE,
    OBJ_RECTANGLE,
    OBJ_CIRCLE,
    OBJ_IMAGE,
    OBJ_RAW_TEXT,
}objType;

typedef enum{
    ERROR_OUT_OF_MEM,

    ERROR_INTERNAL,
}ERRORS;

typedef enum{
    COORD_TERMINAL_SCREEN,
    COORD_NORMALIZED
}coordinateType;

typedef struct {
    coordinateType type;
    double x;
    double y;
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
    terminalInfo oldTerminalInfo;
    Vec objs; // vector of objects
    Screen screen;
    objColor bgColor;
    void (*handleErrors)(ERRORS error , char* msg);
    
}Discky;

void callErrorHandle(const Discky* discky , const ERRORS error , const char* msg);