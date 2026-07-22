#include "../include/colors.h"
#include  "../include/discky.h"
#include <iostream>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#endif

void iniTerminal(){
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif
    std::ios::sync_with_stdio(false);
    std::cout<<"\033[?25l";
    std::cout.flush();
}

Discky::Discky(){
    terminalInfo.sysName = OperatingSystem::INVALID;
    terminalInfo.x = -1;
    terminalInfo.y = -1;
    handleErrors = nullptr;
    bgColor = DISCKY_COLOR_BLACK;
}




void Discky::callErrorHandle(const ERRORS error ,  const std::string& msg)const{
    if(handleErrors)handleErrors(error , msg);
    std::exit(0);
}

void Discky::setErrorHandleFunc(std::function<void(ERRORS , const std::string&)> handler){
    handleErrors = handler;
}

void Discky::setBackground(const objColor&  color){
    bgColor = color;
}







objects& Discky::drawRec(const Coordinate& a , const Coordinate& b ,const objColor& color){
    objects obj;
    obj.color = color;
    obj.type = objType::RECTANGLE;
    obj.vertex.push_back(a);
    obj.vertex.push_back(b);
    objs.push_back(obj);
    return objs.back();
}



void Discky::refresh(){
    objs.clear();
}