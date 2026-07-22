#include "../include/render.h"
#include "../include/discky.h"


static void normalizedToScreen(Discky& discky ,Coordinate& coor){
    if(coor.type == CoordinateType::TERMINAL_SCREEN)return;
    coor.x = ((coor.norX + (double)1.0) * (double)(discky.terminalInfo.x -1))/ (double)2.0;
    coor.y = ((coor.norY + (double)1.0) * (double)(discky.terminalInfo.y -1))/ (double)2.0;
}

static void renderRectangleObj(Discky& discky , objects& obj){
    if(obj.vertex.size()<2)discky.callErrorHandle(ERRORS::INTERNAL ,  "vertex underflow");
    Coordinate& a = obj.vertex[0];
    Coordinate& b = obj.vertex[1];
    normalizedToScreen(discky , a);
    normalizedToScreen(discky , b);

    int tx = std::max(a.x , b.x);
    int ty = std::min(a.y , b.y);
    int bx = std::min(a.x , b.x);
    int by = std::max(a.y , b.y);


    for(int i=ty ; i<=by ; i++){
        for(int j=bx ; j<=tx ; j++)discky.frontBuffer.pixels[i*discky.terminalInfo.x + j] = obj.color;
    }
}


static void renderObj(Discky& discky , objects& obj){
    switch(obj.type){
        case (objType::RECTANGLE):renderRectangleObj(discky , obj);break;
        default: break;
    }
}

void renderNewBg(Discky& discky , int x , int y , Screen& screen){
    screen.x=x;
    screen.y=y;
    screen.pixels.assign(static_cast<size_t>(x)*y , discky.bgColor);
}

void Discky::render(){
    TerminalInfo Info = getTerminalInfo();
    
    if(Info.sysName==OperatingSystem::INVALID)callErrorHandle(ERRORS::TERMINAL_INFO , "failed to get terminal info");

    if((terminalInfo.sysName == OperatingSystem::INVALID)||(terminalInfo.x !=Info.x)||(terminalInfo.y!=Info.y)){
        terminalInfo = Info;
        renderNewBg(*this ,  Info.x ,Info.y , frontBuffer);
        renderNewBg(*this , Info.x , Info.y , backBuffer);
    }

    for(auto& obj:objs)renderObj(*this , obj);
}