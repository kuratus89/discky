#include  "../include/discky.h"





static inline void callErrorHandle(const Discky *discky ,const ERRORS error , const char* msg){
    #ifdef DEBUG
    if(discky->handleErrors!=NULL)discky->handleErrors(error , msg);
    #endif
    exit(0);
}

void setErrorHandleFunc(Discky *discky , void(*handleError)(ERRORS error , char* msg)){
    discky->handleErrors = handleError;
}

void setDisckyBackground(Discky* discky ,const objColor  color){
    discky->bgColor = color;
}







void DisckyDrawRec(Discky *discky ,const coordinate x , const coordinate y ,const objColor color){
    objects* obj = pushVec(discky , &discky->objs);
    iniVec(&obj->vertex , sizeof(coordinate));
    obj->color = color;
    obj->type = OBJ_RECTANGLE;
    *(coordinate*)pushVec(discky , &obj->vertex) = x;
    *(coordinate*)pushVec(discky , &obj->vertex) = y;
}




inline void freeObj(objects* obj){
    free(obj->vertex.vector);
    obj->vertex.count=0;
}

inline void refreshDiscky(Discky* discky){
    for(int i=0 ; discky->objs.count !=i ; i++)freeObj((objects*)getVecElement(discky , &discky->objs , i));
    discky->objs.count=0;
}

void endDiscky(Discky* discky){
    refreshDiscky(discky);
    resizeVec(discky , &discky->objs , 0);
}



void iniScreen(Screen* screen){
    screen->x =0;
    screen->y=0;
    iniVec(&screen->pixels , sizeof(objColor));
}



void iniDiscky(Discky *discky ){
    // discky->oldTerminalInfo.sysName = NULL;
    // discky->oldTerminalInfo.x=-1;
    // discky->oldTerminalInfo.y =-1;
    discky->terminalInfo.sysName=OS_INVALID;
    discky->terminalInfo.x=-1;
    discky->terminalInfo.y=-1;
    discky->handleErrors = NULL;
    iniVec(&discky->objs , sizeof(objects));
    allocBuffer(&discky->frontBuffer);
    allocBuffer(&discky->backBuffer);
    iniScreen(discky->frontBuffer);
    iniScreen(discky->backBuffer);
    iniScreen(&discky->bgBuffer);
    // discky->bgColor = 

}

