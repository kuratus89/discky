#include "../include/render.h"
#include "../include/discky.h"


static void normalizedToScreen(Discky& discky ,Coordinate& coor){
    if((coor.type == COOR_PIX)||(coor.type == LEN_PIX)){
        if((coor.x<0)||(coor.y<0))discky.callErrorHandle(ERRORS::INVALID_INPUT , "invalid coordinate");
        return;
    }

    coor.x = ((coor.norX + (double)1.0) * (double)(discky.terminalInfo.x -1))/ (double)2.0;
    coor.y = ((coor.norY + (double)1.0) * (double)(discky.terminalInfo.y -1))/ (double)2.0;
}

static void drawRange(Discky& discky , int a , int b , int y , objColor color){
    if((y<0)||(y>=discky.terminalInfo.y))return;
    a = std::max(a , 0);
    b = std::min(b , discky.terminalInfo.x-1);
    for(int i=a ;i<=b ; i++)discky.frontBuffer.pixels[y*discky.terminalInfo.x + i] = color;
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

    bx = std::max(bx , 0);
    by = std::min(by , discky.terminalInfo.y -1);
    tx = std::min(tx , discky.terminalInfo.x -1);
    ty = std::max(ty , 0);

    if((bx>tx)||(ty>by))return;

    for(int i=ty ; i<=by ; i++){
        for(int j=bx ; j<=tx ; j++)discky.frontBuffer.pixels[i*discky.terminalInfo.x + j] = obj.color;
    }
}

static void renderCircleObj(Discky& discky , objects& obj){
    if(obj.vertex.size()<2)discky.callErrorHandle(ERRORS::INTERNAL , "vertex underflow");
    Coordinate& a = obj.vertex[0];
    Coordinate& rc = obj.vertex[1];
    normalizedToScreen(discky , a);
    if((rc.type !=LEN_PIX)||(rc.x<0))discky.callErrorHandle(ERRORS::INVALID_INPUT , "invalid radius");

    int x=0;
    int y= rc.x;
    int d = 1-rc.x;
    while(x<=y){
        drawRange(discky , a.x -x , a.x + x , a.y+y , obj.color);
        drawRange(discky , a.x -x , a.x + x , a.y -y , obj.color);
        drawRange(discky , a.x -y , a.x +y , a.y+ x , obj.color);
        drawRange(discky , a.x -y , a.x +y , a.y -x , obj.color);

        if(d<0){
            d+=(2*x) +3;
        }
        else {
            d+= 2*(x-y)+5;
            y--;
        }
        x++;
    }
}

static void renderObj(Discky& discky , objects& obj){
    switch(obj.type){
        case (objType::RECTANGLE):renderRectangleObj(discky , obj);break;
        case (objType::CIRCLE):renderCircleObj(discky , obj);break;
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
        forceRedraw=1;
    }
    else {
        std::fill(frontBuffer.pixels.begin() , frontBuffer.pixels.end() , bgColor);
    }

    for(auto& obj:objs)renderObj(*this , obj);
}