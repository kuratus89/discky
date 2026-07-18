#include  "../include/discky.h"
#include "../include/window.h"

void iniDiscky(Discky *discky ){
    discky->oldTerminalInfo = getTerminalInfo();
    discky->handleErrors = NULL;
    iniVec(&discky->objs , sizeof(objects));
    

}

static inline void callErrorHandle(const Discky *discky ,const ERRORS error , const char* msg){
    if(discky->handleErrors!=NULL)discky->handleErrors(error , msg);
    exit(0);
}

void setErrorHandleFunc(Discky *discky , void(*handleError)(ERRORS error , char* msg)){
    discky->handleErrors = handleError;
}

void setDisckyBackground(objColor  color){

}

void endDiscky(){

}

void updateDiscky(){

}

void clearDiscky(){

}

void pushSquare(Discky *discky , double x , double y , objColor color){
    
}