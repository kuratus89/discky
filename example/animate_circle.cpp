#include <iostream>
#include <thread>
#include <chrono>
#include "../include/discky.h"

void handleDisckyError(ERRORS error , const std::string& msg){
    std::cerr<<msg;
}

void handleBoundryCollision(Discky& discky , int obj , double posx , double posy , double& speedx , double& speedy){
    if(discky.isObjectTouchingBoundryX(obj)){
        if(((posx>0)&&(speedx >0))||((posx<0)&&(speedx<0)))speedx = -speedx;
        
    }
    if(discky.isObjectTouchingBoundryY(obj)){
        if(((posy>0)&&(speedy>0))||((posy<0)&&(speedy<0)))speedy = -speedy;
    }
}

void handleObjectCollision(Discky& discky , int a , int b , double& aSpeedX , double& aSpeedY , double& bSpeedX , double& bSpeedY ){
    if(discky.checkOverlap(a , b)){
        std::swap(aSpeedX , bSpeedX);
        std::swap(aSpeedY , bSpeedY);
    }
}

int main(){
    iniTerminal();
    Discky discky;
    discky.setErrorHandleFunc(handleDisckyError);
    discky.setBackground(DISCKY_COLOR_BLUE);
    discky.setAntiAliasing(antiAliasing::AA_HIGH);
    double ax = -0.5;
    double ay = 0.0;

    double bx = 0.5;
    double by = 0.5;

    double aSpeedX = 0.05;
    double aSpeedY = 0.02;

    double bSpeedX = 0.06;
    double bSpeedY = 0.04;

    

    while(true){
        objId a = discky.drawCircle(NOR_COORD(ax, ay) , NOR_LEN_MONO(0.175) , DISCKY_COLOR_GREEN);
        objId b = discky.drawCircle(NOR_COORD(bx , by) , NOR_LEN_MONO(0.175) , DISCKY_COLOR_RED);
        discky.render();
        handleObjectCollision(discky , a , b , aSpeedX , aSpeedY , bSpeedX , bSpeedY);
        
        handleBoundryCollision(discky , a , ax , ay , aSpeedX , aSpeedY);
        handleBoundryCollision(discky , b , bx , by , bSpeedX , bSpeedY);

        
        ax+=aSpeedX;
        ay+=aSpeedY;
        bx+=bSpeedX;
        by+=bSpeedY;
        
        discky.refresh();
        discky.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}