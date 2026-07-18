#pragma once
#include "window.h"
#include "memory.h"

typedef enum{
    OBJ_SQUARE,
    OBJ_TRIANGLE,
    OBJ_RECTANGLE,
    OBJ_CIRCLE,
    OBJ_IMAGE,
}objType;

typedef enum{
    ERROR_OUT_OF_MEM,
}ERRORS;

typedef struct{
    int r;
    int g;
    int b;
}objColor;

typedef struct{
    Vec vertex;
    objType type;
    objColor color;
}objects;



typedef struct{
    terminalInfo oldTerminalInfo;
    Vec objs;
    Vec screen;
    void (*handleErrors)(ERRORS error , char* msg);
}Discky;

void callErrorHandle(const Discky* discky , const ERRORS error , const char* msg);