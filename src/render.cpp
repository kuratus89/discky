#include "../include/render.h"
#include "../include/discky.h"
#include <algorithm>
#include <cmath>

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
static void drawPoint(Discky& discky , int x , int y , objColor color){
    discky.frontBuffer.pixels[y*discky.terminalInfo.x + x] = color;
}

static void fillFlatTriangle(Discky& discky , double& x1 , double& y1 , double& x2 , double& y2 , double& x3 , double& y3 ,const objColor& color){
    int sy = std::max(0 , (int)ceil(std::min(y1 , y2)));
    int ey = std::min(discky.terminalInfo.y -1 , (int)std::floor(std::max(y1 , y2)));
    if(y1==y2)return;

    double is1 = (x2 - x1)/(y2 - y1);
    double is2 = (x3 - x1)/(y3 - y1);

    for(int y = sy ; y<= ey ; y++){
        double t = y - y1;
        double xa = x1 + (is1*t);
        double xb = x1 + (is2*t);
        drawRange(discky , (int)std::round(std::min(xa , xb)) , (int)std::round(std::max(xa ,xb)) , y , color);
    }
}

static void renderRectangleHorizontalObj(Discky& discky , objects& obj){
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
        // for(int j=bx ; j<=tx ; j++)discky.frontBuffer.pixels[i*discky.terminalInfo.x + j] = obj.color;
        drawRange(discky , bx , tx , i , obj.color);
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

static void renderLine(Discky& discky , objects& obj){
    Coordinate& a = obj.vertex[0];
    Coordinate& b = obj.vertex[1];
    normalizedToScreen(discky , a);
    normalizedToScreen(discky , b);
    int x1 = a.x;
    int y1 = a.y;
    int x2 = b.x;
    int y2 = b.y;
    int dx = abs(x2 - x1);
    int dy = abs(y2-y1);
    int err = dx - dy;
    int sx , sy;
    if(x1<x2)sx=1;
    else sx=-1;
    if(y1<y2)sy=1;
    else sy=-1;
    while(1){
        drawPoint(discky , x1 , y1 , obj.color);
        if((x1 ==x2)&&(y1==y2))break;
        int e2 = err*2;
        if(e2>-dy){
            err -= dy;
            x1+=sx;
        }
        if(e2<dx){
            err+=dx;
            y1+=sy;
        }        
    }
}

static void renderPoly(Discky& discky , objects& obj){
    int n = obj.vertex.size();
    if(n<3)discky.callErrorHandle(ERRORS::INTERNAL , "vertex underflow");
    std::vector<double> cx(n) , cy(n);
    for(int i=0 ; i<n ; i++){
        normalizedToScreen(discky , obj.vertex[i]);
        cx[i] = obj.vertex[i].x;
        cy[i] = obj.vertex[i].y;
    }
    double fymin = cy[0] , fymax = cy[0];
    for(int i=1 ; i<n ; i++){
        fymin = std::min(fymin , cy[i]);
        fymax = std::max(fymax , cy[i]);
    }
    int sy = std::max(0 ,(int) std::ceil(fymin));
    int ey = std::min(discky.terminalInfo.y -1 , (int)std::floor(fymax));
    std::vector<double> sx(n);
    for(int y = sy ; y<=ey ; y++){
        int cnt=0;
        for(int i=0 ; i<n ; i++){
            int j = (i+1)%n;
            double y0 = cy[i];
            double y1 = cy[j];
            if(y0==y1)continue;
            if((y>=std::min(y0 , y1))&&(y<std::max(y0 , y1))){
                sx[cnt] = cx[i] + ((y - y0)/(y1 - y0))*(cx[j]- cx[i]);
                cnt++;
            }
        }
        if(cnt<2)continue;
        std::sort(sx.begin() , sx.begin()+cnt);
        for(int k=0 ; (k+1)<cnt ; k+=2)drawRange(discky , (int)std::round(sx[k]) , (int)std::round(sx[k+1]) , y , obj.color);
    }
}

static void renderTriangleObj(Discky& discky , objects& obj){
    if(obj.vertex.size()<3)discky.callErrorHandle(ERRORS::INTERNAL , "vertex underflow");
    for(int i=0 ; i<3 ; i++)normalizedToScreen(discky , obj.vertex[i]);

    double x1 = obj.vertex[0].x;
    double x2 = obj.vertex[1].x;
    double x3 = obj.vertex[2].x;
    double y1 = obj.vertex[0].y;
    double y2 = obj.vertex[1].y;
    double y3 = obj.vertex[2].y;

    if(y1>y2){
        std::swap(x1 , x2);
        std::swap(y1 ,y2);
    }
    if(y2>y3){
        std::swap(x2 , x3);
        std::swap(y2 , y3);
    }
    if(y1>y2){
        std::swap(x1 , x2);
        std::swap(y1 , y2);
    }

    if(y2==y3)fillFlatTriangle(discky , x1 , y1 , x2 , y2 , x3 , y3 , obj.color);
    else if(y1==y2)fillFlatTriangle(discky , x3 , y3 , x1 , y1 ,x2 , y2 , obj.color);
    else {
        double xs = x1 + (((y2 - y1)/(y3 - y1)) *(x3 - x1));
        fillFlatTriangle(discky , x1 , y1 , x2 , y2 , xs , y2 , obj.color);
        fillFlatTriangle(discky , x3 , y3 , x2 , y2 , xs , y2 , obj.color);
    }
}

static void renderObj(Discky& discky , objects& obj){
    if(obj.removed)return;
    switch(obj.type){
        case (objType::RECTANGLE):renderRectangleHorizontalObj(discky , obj);break;
        case (objType::CIRCLE):renderCircleObj(discky , obj);break;
        case (objType::LINE):renderLine(discky , obj);break;
        case (objType::POLY):renderPoly(discky , obj);break;
        case (objType::TRIANGLE):renderTriangleObj(discky , obj); break;
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