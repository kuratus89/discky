#include "../include/render.h"
#include "../include/discky.h"
#include <algorithm>
#include <cmath>
#include <functional>
using ist = std::function<bool(double , double)>;


static void normalizedToScreenLength(Discky& discky , Coordinate& coor){
    
    if(coor.type ==LEN_PIX){
        if(coor.y==-1)coor.y = coor.x;
        return;
    }

    if(coor.norY ==-1){
        int m = std::min(discky.terminalInfo.x  ,discky.terminalInfo.y) -1;
        coor.x = coor.y = (coor.norX * m);
        return;
    }
    coor.x = (coor.norX * (discky.terminalInfo.x-1));
    coor.y = (coor.norY * (discky.terminalInfo.y -1));
}

static void normalizedToScreen(Discky& discky ,Coordinate& coor){
    if(coor.type == COOR_PIX)return;
    if((coor.type == LEN_NOR)||(coor.type ==LEN_PIX)){
        normalizedToScreenLength(discky , coor);
        return;
    }
    

    coor.x = ((coor.norX + (double)1.0) * (double)(discky.terminalInfo.x -1))/ (double)2.0;
    coor.y = ((coor.norY + (double)1.0) * (double)(discky.terminalInfo.y -1))/ (double)2.0;
}
static objColor blendColor(const objColor& obj , const objColor& src , double alpha){
    alpha = std::max(0.0 , std::min(1.0 , alpha));
    objColor out;
    out.r = (int)std::lround(src.r*alpha + obj.r*(1.0 - alpha));
    out.g = (int)std::lround(src.g*alpha + obj.g*(1.0 - alpha));
    out.b = (int)std::lround(src.b*alpha + obj.b*(1.0 - alpha));
    out.o = 255;
    return out;
}

static void blendPoint(Discky& discky , int x , int y , const objColor& color , double alpha){
    if((x<0)||(y<0)||(x>=discky.terminalInfo.x)||(y>=discky.terminalInfo.y))return;
    if(alpha>=1.0){
        discky.frontBuffer.pixels[y*discky.terminalInfo.x + x] = color;
        return;
    }
    if(alpha<=0.0)return;
    discky.frontBuffer.pixels[y*discky.terminalInfo.x + x] = blendColor(discky.frontBuffer.pixels[y*discky.terminalInfo.x + x], color , alpha);
}

static void blendRange(Discky& discky , int a , int b, int y , const objColor& color ,double alpha){
    if((y<0)||(y>=discky.terminalInfo.y))return;
    a = std::max(a , 0);
    b = std::min(b , discky.terminalInfo.x - 1);
    for(int i=a ; i<=b ; i++)blendPoint(discky , i , y , color , alpha);
}

static int aaSamples(const antiAliasing& mode){
    switch(mode){
        case(antiAliasing::AA_LOW):return 2;
        case (antiAliasing::AA_MEDIUM):return 4;
        case (antiAliasing::AA_HIGH):return 8;
        case (antiAliasing::AA_NONE):return 1;
    }
    return 1;
}

static double sampleCoverage(const ist& tsi , int x , int y , int n){
    int hit = 0;
    double step = 1.0/n;
    for(int j=0 ; j<n ; j++){
        for(int i=0 ; i<n ; i++){
            double sx = x+ (i+0.5)*step;
            double sy = y+(j+0.5)*step;
            if(tsi(sx , sy))hit++;
        }
    }
    return (double)hit/(double)(n*n);
}

static void raster(Discky& discky , const objects& obj , int minX , int maxX, int minY , int maxY , const ist& tsi){
    int n = aaSamples(discky.aaMode);
    minX = std::max(minX , 0);
    maxX = std::min(maxX , discky.terminalInfo.x - 1);
    minY = std::max(0 , minY);
    maxY = std::min(maxY , discky.terminalInfo.y -1);

    for(int y= minY ; y<=maxY ; y++){
        for(int x = minX ; x<=maxX ; x++){
            if(n<=1){
                if(tsi(x+0.5 , y+0.5))blendPoint(discky , x , y , obj.color , obj.opacity);
                continue;
            }
            bool cx1 = tsi(x , y);
            bool cy1 = tsi(x+1 , y);
            bool cx2 = tsi(x , y+1);
            bool cy2 = tsi(x+1 , y+1);
            if((!cx1)&&(!cy1)&&(!cx2)&&(!cy2))continue;
            else {
                double cov = sampleCoverage(tsi, x , y , n);
                if(cov>0.0)blendPoint(discky , x , y , obj.color , obj.opacity*cov);
            }
        }
    }
}

static void renderRectangleObj(Discky& discky , objects& obj){
    if(obj.vertex.size()<2)discky.callErrorHandle(ERRORS::INTERNAL ,"vertex underflow");
    normalizedToScreen(discky , obj.vertex[0]);
    normalizedToScreen(discky , obj.vertex[1]);

    int tx = std::max(obj.vertex[0].x , obj.vertex[1].x);
    int ty = std::min(obj.vertex[0].y , obj.vertex[1].y);
    int bx = std::min(obj.vertex[0].x , obj.vertex[1].x);
    int by = std::max(obj.vertex[0].y , obj.vertex[1].y);

    obj.minX = bx;
    obj.maxX = tx;
    obj.minY = ty;
    obj.maxY = by;

    bx = std::max(bx , 0);
    by = std::min(by , discky.terminalInfo.y - 1);
    tx = std::min(tx , discky.terminalInfo.x -1);
    ty = std::max(ty , 0);

    

    if((bx>tx)||(ty>by))return;
    if(obj.boder>=1.0){
        for(int i=ty ; i<=by ; i++)blendRange(discky , bx , tx , i , obj.color , obj.opacity);
        return;
    }

    int w = tx - bx+1;
    int h = by - ty +1;
    int hx = std::max(1 , (int)std::lround(obj.boder*(w/2.0)));
    int hy = std::max(1 , (int)std::lround(obj.boder*(h/2.0)));

    for(int i=ty ; i<=by ; i++){
        if((i<ty+hy)||(i>by - hy)){
            blendRange(discky , bx , tx , i , obj.color , obj.opacity);
        }
        else {
            blendRange(discky , bx , bx+hx- 1 , i , obj.color , obj.opacity);
            blendRange(discky , tx - hx+1 , tx , i , obj.color , obj.opacity);
        }
    }
}

static void renderCircleObj(Discky& discky , objects& obj){
    if(obj.vertex.size()<2)discky.callErrorHandle(ERRORS::INTERNAL , "vertex underflow");
    normalizedToScreen(discky , obj.vertex[0]);
    normalizedToScreen(discky , obj.vertex[1]);

    double rx = obj.vertex[1].x;
    double ry = obj.vertex[1].y;
    double ir = 0.0;
    if(obj.boder<1.0)ir = 1.0 - obj.boder;

    double cx = obj.vertex[0].x;
    double cy = obj.vertex[0].y;

    ist tsi = [cx , cy , rx , ry , ir](double px , double py)->bool{
        if((rx<=0.0)||(ry<=0.0))return 0;
        double dx = (px - cx)/rx;
        double dy = (py - cy)/ry;
        double d = (dx*dx) + (dy*dy);
        if(d>1.0)return 0;
        if(ir>0.0){
            double idx = dx/ir;
            double idy = dy/ir;
            if(((idx*idx) + (idy*idy) )<1.0)return 0;
        }
        return 1;
    };
    obj.minX = (int)std::floor(cx - rx);
    obj.maxX = (int)std::ceil(cx + rx);
    obj.minY = (int)std::floor(cy - ry);
    obj.maxY = (int)std::ceil(cy + ry);

    raster(discky , obj , obj.minX , obj.maxX , obj.minY , obj.maxY , tsi);
}



static void renderLine(Discky& discky , objects& obj){
    normalizedToScreen(discky , obj.vertex[0]);
    normalizedToScreen(discky , obj.vertex[1]);

    double x1 = obj.vertex[0].x;
    double y1 = obj.vertex[0].y;
    double x2 = obj.vertex[1].x;
    double y2 = obj.vertex[1].y;

    double vx = x2 - x1;
    double vy = y2 - y1;
    double len = (vx*vx) + (vy*vy);
    double hw = 0.5;

    ist tsi = [x1 , y1 , vx , vy , len , hw](double px , double py)->bool{
        double t = 0.0;
        if(len>0.0)t= std::max(0.0 , std::min(1.0 , (((px - x1)*vx) + ((py - y1)*vy))/len));
        double xp = x1 + (t*vx);
        double yp = y1 + (t*vy);
        double dx =px - xp;
        double dy = py - yp;
        return ((dx*dx)+(dy*dy))<=(hw*hw);
    };
    int minX = (int)std::floor(std::min(x1 , x2) - 1);
    int maxX = (int)std::ceil(std::max(x1 , x2)+1);
    int minY = (int)std::floor(std::min(y1 , y2)- 1);
    int maxY = (int)std::ceil(std::max(y1 , y2)+ 1);

    obj.minX = minX;
    obj.maxX = maxX;
    obj.minY = minY;
    obj.maxY = maxY;

    raster(discky , obj , minX , maxX , minY , maxY , tsi);
}

static double triSign(double x1 , double y1 , double x2 , double y2 , double x3 , double y3){
    return ((x1-x3)*(y2 - y3))- ((x2- x3)*(y1-y3));
}

static bool pointInTriangle(double px , double py , double ax , double ay , double bx , double by , double cxx , double cyy){
    double d1 =triSign(px , py , ax , ay , bx , by);
    double d2 =triSign(px , py , bx , by , cxx , cyy);
    double d3 = triSign(px , py , cxx , cyy , ax , ay);
    bool hasNeg = (d1<0)||(d2<0)||(d3<0);
    bool hasPos = (d1>0)||(d2>0)||(d3>0);
    return (!(hasNeg && hasPos));
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

    double mx = (x1+x2+x3)/3.0;
    double my = (y1 + y2+ y3)/3.0;

    bool hasBorder = obj.boder<1.0;
    double scale = 1.0 - obj.boder;
    double ix1 = mx + ((x1 - mx)*scale);
    double ix2 = mx + ((x2 - mx)*scale);
    double ix3 = mx + ((x3 -mx )*scale);
    double iy1 = my + ((y1 - my)*scale);
    double iy2 = my + ((y2 - my)*scale);
    double iy3 = my + ((y3 - my)*scale);

    ist tsi = [=](double px , double py)->bool{
        if(!pointInTriangle(px , py , x1 , y1 , x2 , y2 , x3 , y3))return 0;
        if(hasBorder && pointInTriangle(px , py , ix1 , iy1  , ix2 , iy2 , ix3 , iy3))return 0;
        return 1;
    };

    int minX = (int)std::floor(std::min({x1 , x2 , x3}));
    int maxX = (int)std::ceil(std::max({x1 , x2 , x3}));
    int minY = (int)std::floor(std::min({y1 , y2 , y3}));
    int maxY = (int)std::ceil(std::max({y1 , y2 , y3}));

    obj.minX = minX;
    obj.maxX = maxX;
    obj.minY = minY;
    obj.maxY = maxY;
    raster(discky , obj , minX , maxX , minY , maxY , tsi);
}

static bool pointInPoly(double px , double py , const std::vector<double>& vx , const std::vector<double>& vy){
    int n = (int)vx.size();
    bool ins = 0;
    for(int i=0 , j=n-1 ; i<n ; j=i , i++){
        if((vy[i]>py)!=(vy[j]>py)){
            double xi = (vx[j] - vx[i])* ((py -vy[i])/(vy[j] - vy[i])) + vx[i];
            if(px<xi)ins = !ins;
        }        
    }
    return ins;
}

static void renderPoly(Discky& discky , objects obj){
    int n = obj.vertex.size();
    if(n<3)discky.callErrorHandle(ERRORS::INTERNAL , "vertex underflow");
    std::vector<double> cx(n) , cy(n);
    for(int i=0 ; i<n ; i++){
        normalizedToScreen(discky , obj.vertex[i]);
        cx[i] = obj.vertex[i].x;
        cy[i] = obj.vertex[i].y;
    }
    double fxMin = cx[0];
    double fxMax = cx[0];
    double fyMin = cy[0];
    double fyMax = cy[0];
    double centerX = 0.0;
    double centerY = 0.0;
    for(int i=0 ; i<n ; i++){
        fxMin = std::min(fxMin , cx[i]);
        fxMax = std::max(fxMax , cx[i]);
        fyMin = std::min(fyMin , cy[i]);
        fyMax = std::max(fyMax , cy[i]);
        centerX+=cx[i];
        centerY+=cy[i];
    }
    centerX/=n;
    centerY/=n;
    
    bool hasBoder = obj.boder<1.0;
    std::vector<double> xi(n) , yi(n);
    double scale = 1.0 - obj.boder;
    if(hasBoder){
        for(int i=0 ; i<n ; i++){
            xi[i] = centerX + ((cx[i] - centerX)*scale);
            yi[i] =centerY + ((cy[i] - centerY)*scale);
        }
    }
    ist tsi = [&cx ,&cy , &xi , &yi , hasBoder](double px , double py)->bool{
        if(!pointInPoly(px , py , cx ,cy))return 0;
        if(hasBoder && pointInPoly(px , py , xi , yi))return 0;
        return 1;
    };
    int minX = (int)std::floor(fxMin);
    int maxX = (int)std::ceil(fxMax);
    int minY = (int)std::floor(fyMin);
    int maxY = (int)std::ceil(fyMax);

    obj.minX = minX;
    obj.maxX = maxX;
    obj.minY = minY;
    obj.maxY = maxY;
    raster(discky , obj , minX , maxX , minY , maxY , tsi);
}

static void renderObj(Discky& discky , objects& obj){
    if(obj.removed)return;
    switch(obj.type){
        case (objType::RECTANGLE):renderRectangleObj(discky , obj);break;
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