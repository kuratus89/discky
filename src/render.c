#include "../include/render.h"

void normalizedToScreen(Discky* discky ,coordinate* coor){
    if(coor->type == COORD_TERMINAL_SCREEN)return;
    coor->x = ((coor->norX + (double)1.0) * (double)(discky->terminalInfo.x -1))/ (double)2.0;
    coor->y = ((coor->norY + (double)1.0) * (double)(discky->terminalInfo.y -1))/ (double)2.0;
}

static inline void renderRectangleObj(Discky* discky , objects* obj){
    if(obj->vertex.count<2)callErrorHandle(discky , ERROR_INTERNAL ,  "vertex underflow");
    coordinate* x = getVecElement(discky , &obj->vertex , 0);
    coordinate* y = getVecElement(discky , &obj->vertex , 1);
    normalizedToScreen(discky , x);
    normalizedToScreen(discky , y);
    int tx,ty , bx , by;
    tx = MAX((int) x->x , (int)y->x);
    ty = MIN((int)x->y ,(int) y->y);
    bx = MIN((int)x->x , (int)y->x);
    by = MAX((int)x->y , (int)y->y);


    for(int i = ty ; i<=by ; i++){
        for(int j = bx ; j<=tx ; j++){
            *(objColor*)getVecElement(discky , &discky->frontBuffer->pixels ,i*discky->terminalInfo.x + j ) = obj->color;
        }
    }
}


void renderObj(Discky* discky , objects* obj){
    switch(obj->type){
        case (OBJ_RECTANGLE):
            renderRectangleObj(discky , obj);
            break;
        
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
        renderNewBg(discky , terminalInfo.x , terminalInfo.y , discky->frontBuffer);
        renderNewBg(discky , terminalInfo.x , terminalInfo.y , discky->backBuffer);
    }

    for(int i=0 ; i<discky->objs.count ; i++)renderObj(discky , (objects*)getVecElement(discky , &discky->objs , i));
    


}