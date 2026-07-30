#include "../include/render.h"
#include "../include/discky.h"
#include <algorithm>
#include <cmath>
#include <functional>
using ist = std::function<bool(double , double)>;
struct pxf{
    double px , py;
    double fx , fy;
};

struct clipReact{
    int minX=0;
    int maxX =0;
    int minY=0;
    int maxY =0;
    bool active =0;
    ist shapeTest = nullptr;
};
static clipReact intersectRect(const clipReact& a , int minX , int maxX , int minY , int maxY){
    clipReact r;
    r.active =1;
    if(a.active){
        r.minX = std::max(a.minX , minX);
        r.maxX = std::min(a.maxX , maxX);
        r.minY = std::max(a.minY , minY);
        r.maxY = std::min(a.maxY , maxY);
    }
    else{
        r.minX = minX;
        r.minY = minY;
        r.maxX = maxX;
        r.maxY = maxY;
    }
    return r;
}

static void normalizedToParentLength(Coordinate& coor , const pxf& p){
    if(coor.type == LEN_PIX){
        if(coor.y==-1)coor.y = coor.x;
        return;
    }
    if(coor.norY==-1){
        double m = std::min(p.fx , p.fy);
        coor.x = coor.y = (int)std::lround(coor.norX*m);
        return;
    }
    coor.x = (int)std::lround(coor.norX* p.fx);
    coor.y = (int)std::lround(coor.norY * p.fy);

}



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
static void toScreen(Discky& discky , Coordinate& coor , const pxf* p){
    if(p==nullptr){
        normalizedToScreen(discky, coor);
        return;
    }
    if(coor.type ==COOR_PIX){
        coor.x = (int)std::lround(p->px + coor.x);
        coor.y = (int)std::lround(p->py + coor.y);
        return;
    }
    if((coor.type ==LEN_NOR)||(coor.type==LEN_PIX)){
        normalizedToParentLength(coor , *p);
        return;
    }
    coor.x = (int)std::lround(p->px + (coor.norX * p->fx));
    coor.y = (int)std::lround(p->py + (coor.norY * p->fy));
}
static objColor blendColor(const objColor& obj , const objColor& src , double alpha){
    alpha = std::max(0.0 , std::min(1.0 , alpha));
    objColor out;
    out.r = (int)std::lround(src.r*alpha + obj.r*(1.0 - alpha));
    out.g = (int)std::lround(src.g*alpha + obj.g*(1.0 - alpha));
    out.b = (int)std::lround(src.b*alpha + obj.b*(1.0 - alpha));
    // out.o = 255;
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

static void raster(Discky& discky , const objects& obj , int minX , int maxX, int minY , int maxY , const ist& tsi , const clipReact* clip){
    int n = aaSamples(discky.aaMode);
    minX = std::max(minX , 0);
    maxX = std::min(maxX , discky.terminalInfo.x - 1);
    minY = std::max(0 , minY);
    maxY = std::min(maxY , discky.terminalInfo.y -1);

    if(clip){
        minX = std::max(minX , clip->minX);
        maxX = std::min(maxX , clip->maxX);
        minY = std::max(minY , clip->minY);
        maxY = std::min(maxY ,clip->maxY);
    }

    ist eff = tsi;
    if((clip)&&(clip->shapeTest)){
        ist clipShape = clip->shapeTest;
        eff = [tsi , clipShape](double px , double py)->bool{
            return ((tsi(px , py))&& clipShape(px , py));
        };
    }

    for(int y= minY ; y<=maxY ; y++){
        for(int x = minX ; x<=maxX ; x++){
            if(n<=1){
                if(eff(x+0.5 , y+0.5))blendPoint(discky , x , y , obj.color , obj.opacity);
                continue;
            }
            double cov = sampleCoverage(eff, x , y , n);
            if(cov > 0.0) blendPoint(discky , x , y , obj.color , obj.opacity*cov);
        }
    }
}

static void renderRectangleObj(Discky& discky , objects& obj , const pxf* parent, const clipReact* clip){
    if(obj.vertex.size()<2)discky.callErrorHandle(ERRORS::INTERNAL ,"vertex underflow");
    toScreen(discky , obj.vertex[0] ,parent );
    toScreen(discky , obj.vertex[1] , parent);

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

    if(clip){
        bx = std::max(bx , clip->minX);
        tx = std::min(tx , clip->maxX);
        ty = std::max(ty , clip->minY);
        by = std::min(by , clip->maxY);

    }

    

    if((bx>tx)||(ty>by))return;

    const ist* clipShape;
    if((clip)&&(clip->shapeTest))clipShape = &clip->shapeTest;
    else clipShape = nullptr;

    auto fr = [&](int a , int b , int y){
        if(!clipShape){
            blendRange(discky , a , b , y , obj.color , obj.opacity);
            return;            
        }
        if((y<0)||(y>=discky.terminalInfo.y))return;
        a = std::max(a , 0);
        b = std::min(b , discky.terminalInfo.x - 1);
        for(int i=a ; i<=b ; i++){
            if((*clipShape)(i+0.5 , y+0.5))blendPoint(discky , i , y , obj.color , obj.opacity );
        }
    };

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
            fr(bx , tx , i);
        }
        else {
            fr(bx , bx+hx -1 , i);
            fr(tx - hx+1 , tx , i);
        }
    }
}

static void renderCircleObj(Discky& discky , objects& obj , const pxf* parent , const clipReact* clip){
    if(obj.vertex.size()<2)discky.callErrorHandle(ERRORS::INTERNAL , "vertex underflow");
    toScreen(discky , obj.vertex[0] , parent);
    toScreen(discky , obj.vertex[1],  parent);

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

    raster(discky , obj , obj.minX , obj.maxX , obj.minY , obj.maxY , tsi , clip);
}




static void renderLine(Discky& discky , objects& obj ,  const pxf* parent , const clipReact* clip){
    toScreen(discky , obj.vertex[0] , parent);
    toScreen(discky , obj.vertex[1] , parent);

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

    raster(discky , obj , minX , maxX , minY , maxY , tsi , clip);
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

static void renderTriangleObj(Discky& discky , objects& obj , const pxf* parent , const clipReact* clip){
    if(obj.vertex.size()<3)discky.callErrorHandle(ERRORS::INTERNAL , "vertex underflow");
    for(int i=0 ; i<3 ; i++)toScreen(discky , obj.vertex[i] , parent);

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
    raster(discky , obj , minX , maxX , minY , maxY , tsi , clip);
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

static void renderPoly(Discky& discky , objects obj , const pxf* parent , const clipReact* clip){
    int n = obj.vertex.size();
    if(n<3)discky.callErrorHandle(ERRORS::INTERNAL , "vertex underflow");
    std::vector<double> cx(n) , cy(n);
    for(int i=0 ; i<n ; i++){
        toScreen(discky , obj.vertex[i] , parent);
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
    raster(discky , obj , minX , maxX , minY , maxY , tsi , clip);
}

static ist buildObjShapeTest(const objects& obj){
    switch(obj.type){
        case (objType::CIRCLE):{
            double cx = obj.vertex[0].x;
            double cy = obj.vertex[0].y;
            double rx = obj.vertex[1].x;
            double ry = obj.vertex[1].y;
            double ir = 0.0;
            if(obj.boder<1.0)ir = 1.0 - obj.boder;
            return [cx , cy , rx , ry , ir](double px , double py)->bool{
                if((rx<=0.0)||(ry<=0.0))return 0;
                double dx = (px - cx)/rx;
                double dy = (py - cy)/ry;
                double d = (dx*dx)+(dy*dy);
                if(d>1.0)return 0;
                if(ir>0.0){
                    double idx = dx/ir;
                    double idy = dy/ir;
                    if(((idx*idx)+(idy*idy))<1.0)return 0;
                }
                return 1;
            };
        }
        case (objType::TRIANGLE):{
            double x1 = obj.vertex[0].x;
            double x2 = obj.vertex[1].x;
            double x3 = obj.vertex[2].x;
            double y1 = obj.vertex[0].y;
            double y2 = obj.vertex[1].y;
            double y3 = obj.vertex[2].y;
            return [x1 , y1 , x2 , y2 , x3 , y3](double px , double py)->bool{
                return pointInTriangle(px , py , x1 , y1 , x2 , y2 , x3 , y3);
            };
        }
        case (objType::POLY):{
            std::vector<double> vx , vy;
            vx.reserve(obj.vertex.size());
            vy.reserve(obj.vertex.size());
            for(const auto& v:obj.vertex){
                vx.push_back(v.x);
                vy.push_back(v.y);
            }
            return [vx , vy](double px , double py)->bool{
                return pointInPoly(px , py , vx , vy);
            };
        }

        default:return nullptr;
    }
}

static void renderObj(Discky& discky , objects& obj , const pxf* parent , const clipReact* clip){
    if(obj.removed)return;
    switch(obj.type){
        case (objType::RECTANGLE):renderRectangleObj(discky , obj , parent , clip);break;
        case (objType::CIRCLE):renderCircleObj(discky , obj , parent , clip);break;
        case (objType::LINE):renderLine(discky , obj ,parent , clip );break;
        case (objType::POLY):renderPoly(discky , obj , parent , clip);break;
        case (objType::TRIANGLE):renderTriangleObj(discky , obj , parent , clip); break;
        default: break;
    }

    if(obj.children.empty())return;

    pxf cxf{
        (obj.minX + obj.maxX)/2.0,
        (obj.minY + obj.maxY)/2.0,
        std::max(0.5 , (obj.maxX - obj.minX)/2.0),
        std::max(0.5 , (obj.maxY - obj.minY)/2.0)
    };
    clipReact cp ;
    if(clip)cp = *clip;
    else cp = clipReact();

    if(obj.clipChildren){
        cp = intersectRect(clip ? *clip : clipReact{} , obj.minX , obj.maxX , obj.minY , obj.maxY);

        ist ownShape = buildObjShapeTest(obj);
        if(ownShape){
            if(clip && clip->shapeTest){
                ist inherited = clip->shapeTest;
                cp.shapeTest = [inherited , ownShape](double px , double py)->bool{
                    return inherited(px , py) && ownShape(px , py);   // calls ownShape(px,py), not just `ownShape`
                };
            }
            else {
                cp.shapeTest = ownShape;
            }
        }
    }

    for(objId id:obj.children){
        if(id>=discky.objs.size())continue;
        if(cp.active)renderObj(discky , discky.objs[id] , &cxf , &cp);
        else renderObj(discky , discky.objs[id] , &cxf , nullptr);
    }
}

void renderNewBg(Discky& discky , int x , int y , Screen& screen){
    screen.x=x;
    screen.y=y;
    screen.pixels.assign(static_cast<size_t>(x)*y , discky.bgColor);
}

void renderTxt(Discky& discky , rawText& txt){
    normalizedToScreen(discky , txt.vertex);
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

    for(auto& obj:objs){
        if(obj.parent != NO_PARENT)continue;
        renderObj(*this , obj, nullptr , nullptr);
    }
    for(auto& txt:rawTxts)renderTxt(*this , txt);

}