#include "../include/render.h"

static inline void renderRectangleObj(Discky* discky , objects* obj){
    
}


void renderObj(Discky* discky , objects* obj){
    switch(obj->type){
        case (OBJ_RECTANGLE)
    }
}

void renderNewBg(Discky* discky , int x , int y , Screen* screen){
    screen->x=x;
    screen->y=y;
    int size = x*y;
    resizeVec(discky , &screen->pixels , size);
    for(int i=0 ; i!=size ; i++)*(objColor*)getVecElement(discky , &screen->pixels , i) = discky->bgColor;
}

void renderDiscky(Discky* discky){
    TerminalInfo terminalInfo = getTerminalInfo();
    
    if(terminalInfo.sysName==OS_INVALID)callErrorHandle(discky , ERROR_TERMINAL_INFO , "failed to get terminal info");

    if((discky->terminalInfo.sysName = OS_INVALID)||(discky->terminalInfo.x != terminalInfo.x)||(discky->terminalInfo.y!=terminalInfo.y)){
        discky->terminalInfo = terminalInfo;
        renderNewBg(discky , terminalInfo.x , terminalInfo.y , &discky->bgBuffer);
    }


}