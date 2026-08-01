#include "../include/colors.h"
#include  "../include/discky.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <algorithm>

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

void endDiscky(){
    std::cout<<"\033[?25h\033[0m";
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
    endDiscky();
    std::exit(0);
}

void Discky::setErrorHandleFunc(std::function<void(ERRORS , const std::string&)> handler){
    handleErrors = handler;
}

void Discky::setBackground(const objColor&  color){
    bgColor = color;
}

static objId recycleObjPush(Discky& discky ,objects& obj){
    if(discky.objRecycleBin.size()>0){
        int it = discky.objRecycleBin.back();
        discky.objRecycleBin.pop_back();
        discky.objs[it] = obj;
        return  it;
    }
    discky.objs.push_back(obj);
    return( discky.objs.size()-1);
}

objId Discky::drawRectangle(const Coordinate& a , const Coordinate& b ,const objColor& color , const double& border , const double& opacity){
    objects obj;
    obj.vertex.reserve(2);
    obj.color = color;
    obj.type = objType::RECTANGLE;
    obj.border = std::max(0.0 , std::min(1.0 , border));
    obj.opacity = std::max(0.0 , std::min(1.0 , opacity));
    obj.vertex.push_back(a);
    obj.vertex.push_back(b);
    return recycleObjPush(*this , obj);
}

objId Discky::drawCircle(const Coordinate& a , const Coordinate& b , const objColor& color , const double& border , const double& opacity){
    objects obj;
    obj.vertex.reserve(2);
    obj.color = color;
    obj.type = objType::CIRCLE;
    obj.border = std::max(0.0 , std::min(1.0 , border));
    obj.opacity = std::max(0.0 , std::min(1.0 , opacity));
    obj.vertex.push_back(a);
    obj.vertex.push_back(b);
    return recycleObjPush(*this , obj);
}

objId Discky::drawLine(const Coordinate& a, const Coordinate& b , const objColor& color , const double& opacity){
    objects obj;
    obj.vertex.reserve(2);
    obj.color = color;
    obj.type = objType::LINE;
    obj.opacity = std::max(0.0 , std::min(1.0 , opacity));
    obj.vertex.push_back(a);
    obj.vertex.push_back(b);
    return recycleObjPush(*this , obj);
}

objId Discky::drawPoly(const std::vector<Coordinate> &ver , const objColor &color , const double& border , const double& opacity){
    objects obj;
    if(ver.size()==0)callErrorHandle(ERRORS::INVALID_INPUT , "invalid numbers of vertex for poly");
    obj.vertex.resize(ver.size());
    obj.color = color;
    obj.type = objType::POLY;
    obj.opacity = std::max(0.0 , std::min(1.0 , opacity));
    obj.border = std::max(0.0 , std::min(1.0 , border));
    for(int i=0 ; i<ver.size() ; i++)obj.vertex[i] = ver[i];
    return recycleObjPush(*this , obj);
}

objId Discky::drawTriangle(const Coordinate& a , const Coordinate& b , const Coordinate& c , const objColor & color , const double& border , const double& opacity){
    objects obj;
    obj.vertex = {a , b , c};
    obj.color = color;
    obj.type = objType::TRIANGLE;
    obj.border = std::max(0.0 , std::min(1.0 , border));
    obj.opacity = std::max(0.0 , std::min(1.0 , opacity));
    return recycleObjPush(*this , obj);
}


bool Discky::isObjectTouchingBoundryX(objId obj){
    if((obj<0)||(obj>=objs.size()))return 0;
    if((objs[obj].minX<=0)||(objs[obj].maxX>=terminalInfo.x))return 1;
    return 0;
}

bool Discky::isObjectTouchingBoundryY(objId obj){
    if((obj<0)||(obj>=objs.size()))return 0;
    if((objs[obj].minY<=0)||(objs[obj].maxY>=terminalInfo.y))return 1;
    return 0;
}

void Discky::setAntiAliasing(const antiAliasing& mode){
    aaMode = mode;
}

void Discky::removeObj(objects& obj){
    objId id = (objId)(&obj - objs.data());

    if((obj.parent != NO_PARENT)&&(obj.parent<objs.size())){
        auto& si = objs[obj.parent].children;
        si.erase(std::remove(si.begin() , si.end() , id));

    }
    for(objId c:obj.children)if(c<objs.size())objs[c].parent = NO_PARENT;
    obj.children.clear();

    obj.removed =1;
    objRecycleBin.push_back(id);
}

void Discky::addChild(objId parent , objId child){
    if((parent>=objs.size())||(child>=objs.size())){
        callErrorHandle(ERRORS::INVALID_INPUT , "addChild : invalid parent/child id");
    }
    if(parent ==child)callErrorHandle(ERRORS::INVALID_INPUT , "addChild : object cannot be its own parent");
    objId wk = parent;
    while(wk !=NO_PARENT){
        if(wk == child)callErrorHandle(ERRORS::INVALID_INPUT , "addChild : a child cannot be its own ancestor");
        wk = objs[wk].parent;
    }
    if(objs[child].parent !=NO_PARENT){
        auto si = objs[objs[child].parent].children;
        si.erase(std::remove(si.begin() , si.end() , child) , si.end());
    }
    objs[child].parent = parent;
    objs[parent].children.push_back(child);
}
int Discky::getTerminalSizeX(){
    return terminalInfo.x;
}

int Discky::getTerminalSizeY(){
    return terminalInfo.y;
}

void Discky::refresh(){
    objs.clear();
    rawTxts.clear();
}

void objects::removeObj(){
    removed=1;
}

struct pt{
    double x;
    double y;
};

std::vector<pt> getPolyPoints(const objects& obj){
    std::vector<pt> pts;
    switch(obj.type){
        case (objType::RECTANGLE):{
            double x1 = obj.vertex[0].x;
            double x2 = obj.vertex[1].x;
            double y1 = obj.vertex[0].y;
            double y2 = obj.vertex[1].y;
            double lx = std::min(x1 , x2);
            double ly = std::min(y1 , y2);
            double hx = std::max(x1 , x2);
            double hy = std::max(y1 , y2);
            pts = {{lx , ly} , {hx , ly} , {hx , hy} , {lx , hy}};
            break;
        }
        case (objType::TRIANGLE):
        case (objType::POLY):{
            for(const auto& v:obj.vertex)pts.push_back({(double)v.x , (double) v.y});
            break;
        }
        case (objType::LINE):{
            pts = {{(double)obj.vertex[0].x , (double)obj.vertex[0].y} , {(double) obj.vertex[1].x , (double)obj.vertex[1].y}};
            break;
        }
        default:break;
    }
    return pts;
}


bool pointInPoly(const pt& p , const std::vector<pt>& poly){
    int n = poly.size();
    if(n<3)return 0;
    bool ins = 0;
    for(int i=0 , j = n-1 ; i<n ; j=i , i++){
        if((poly[i].y>p.y)!=(poly[j].y>p.y)){
            double xi  = (poly[j].x -poly[i].x)* ((p.y- poly[i].y)/(poly[j].y - poly[i].y)) + poly[i].x;
            if(p.x < xi)ins= !ins;
        }
    }
    return ins;
}
void projectPolygon(const std::vector<pt>& poly , double ax ,double ay , double& mn , double& mx){
    mn = mx = (poly[0].x*ax) + (poly[0].y*ay);
    for(int i= 1 ; i<poly.size() ; i++){
        double p = (poly[i].x*ax)+(poly[i].y*ay);
        mn = std::min(mn , p);
        mx = std::max(mx , p);
    }
}
bool satOverlap(const std::vector<pt>& a , const std::vector<pt>& b){
    for(int i = 0; i<2 ; i++){
        const std::vector<pt>& poly = (i==0)? a:b;
        int n = poly.size();
        int e;
        if(n==2)e=1;
        else e = n;
        
        for(int i=0 ; i<e ; i++){
            int j = (i+1)%n;
            double ex = poly[j].x - poly[i].x;
            double ey = poly[j].y - poly[i].y;
            double ax = -ey;
            double ay = ex;
            double len = std::sqrt((ax*ax)+(ay*ay));
            ax/=len;
            ay/=len;
            double mina , maxa , minb , maxb;
            projectPolygon(a , ax , ay , mina , maxa);
            projectPolygon(b , ax , ay , minb , maxb);
            if((maxa<minb)||(maxb<mina))return 0;
        }
    }
    return 1;
}

bool pointEllip(const pt& p , double cx , double cy , double rx , double ry){
    if((rx<=0.0)||(ry<=0.0))return 0;
    double dx = (p.x - cx)/rx;
    double dy = (p.y - cy)/ry;
    return (((dx*dx)+(dy*dy))<=1.0);
}

double clsPointOnSegmentDistSq(const pt& p1 , const pt& p2 , const pt& org){
    double vx = p2.x - p1.x;
    double vy = p2.y - p1.y;
    double len = ((vx*vx) + (vy*vy));
    double t = 0.0;
    if(len>1e-12)t = std::max(0.0 , std::min(1.0 , (((org.x-p1.x)*vx)+((org.y-p1.y)*vy))/len));
    double cx = p1.x + (t*vx);
    double cy = p1.y + (t*vy);
    double dx = cx - org.x;
    double dy = cy-org.y;
    return ((dx*dx)+ (dy*dy));
}

bool segIntersectEllip(const pt& p1 , const pt& p2 , double cx , double cy , double rx , double ry){
    if((rx<=0.0)||(ry<=0.0))return 0;
    pt tp1{((p1.x - cx)/rx) , ((p1.y - cy)/ry)};
    pt tp2{((p2.x - cx)/rx) , ((p2.y - cy)/ry)};
    pt org{0.0 , 0.0};
    return (clsPointOnSegmentDistSq(tp1 , tp2 , org)<=1.0);
}

bool circleCircleOverlap(const objects& a , const objects& b){
    double ax = a.vertex[0].x;
    double ay = a.vertex[0].y;
    double bx = b.vertex[0].x;
    double by = b.vertex[0].y;
    double ra = (a.vertex[1].x + a.vertex[1].y)/2.0;
    double rb = (b.vertex[1].x + b.vertex[1].y)/2.0;

    double dx = ax - bx;
    double dy = ay - by;
    double dist = std::sqrt((dx*dx)+(dy*dy));
    return (dist<=(ra+rb));
}

bool circlePolyOverlap(const objects& circle , const objects& poly){
    double cx = circle.vertex[0].x;
    double cy = circle.vertex[0].y;
    double rx = circle.vertex[1].x;
    double ry = circle.vertex[1].y;

    std::vector<pt> pts = getPolyPoints(poly);
    if(pts.empty())return 0;
    if(pointInPoly({cx , cy} , pts))return 1;

    for(const auto& p:pts)if(pointEllip(p , cx , cy , rx , ry))return 1;

    int n = pts.size();
    int edge;
    if(n==2)edge = 1;
    else edge = n;

    for(int i=0; i<edge ; i++){
        int j = (i+1)%n;
        if(segIntersectEllip(pts[i] , pts[j] , cx , cy , rx , ry))return 1;

    }
    return 0;
}

bool Discky::checkOverlap(const objId ia ,const objId ib){
    if((ia<0)||(ib<0)||(ia>=objs.size())||(ib>=objs.size()))return 0;
    objects& a  = objs[ia];
    objects& b = objs[ib];
    
    bool ac = (a.type ==objType::CIRCLE);
    bool bc = (b.type == objType::CIRCLE);
    if(ac && bc)return circleCircleOverlap(a, b);
    if(ac)return circlePolyOverlap(a , b);
    if(bc)return circlePolyOverlap(b , a);

    std::vector<pt> pa = getPolyPoints(a);
    std::vector<pt> pb = getPolyPoints(b);
    if((pa.empty())||(pb.empty()))return 0;

    return satOverlap(pa , pb);
}

objId Discky::drawRawTxt(const Coordinate& a , const std::string& txt , const objColor& color){
    rawText text;
    text.color = color;
    text.txt =txt;
    text.vertex = a;
    rawTxts.push_back(text);
    return (rawTxts.size()-1);
}

bool Discky::isTxtTouchingBoundryX(objId txt){
    if((txt<0)||(txt>=rawTxts.size()))return 0;
    if((rawTxts[txt].vertex.x <=0)||(rawTxts[txt].vertex.x+ rawTxts[txt].txt.size()>=terminalInfo.x ))return 1;
    return 0;
}

bool Discky::isTxtTouchingBoundryY(objId txt){
    if((txt<0)||(txt>=rawTxts.size()))return 0;
    if((rawTxts[txt].vertex.y <=0)||(rawTxts[txt].vertex.y >= (terminalInfo.y/2)))return 1;
    return 0;
}