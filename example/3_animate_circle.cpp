#include <iostream>
#include <thread>
#include <chrono>
#include "../include/discky.h"

void handleDisckyError(ERRORS error , const std::string& msg){
    std::cerr<<msg;
}

int main(){
    iniTerminal();
    Discky discky;
    discky.setErrorHandleFunc(handleDisckyError);
    discky.setBackground(DISCKY_COLOR_BLUE);
    // discky.setAntiAliasing(antiAliasing::AA_HIGH);
    double ax = -0.5;
    double ay = 0.0;

    double bx = 0.5;
    double by = 0.0;

    double aSpeedX = 0.05;
    double aSpeedY = 0.02;

    double bSpeedX = 0.05;
    double bSpeedY = 0.02;

    

    while(true){
        objects& a = discky.drawCircle(NOR_COORD(ax, ay) , NOR_LEN_MONO(0.1) , DISCKY_COLOR_GREEN);
        objects& b = discky.drawCircle(NOR_COORD(bx , by) , NOR_LEN_MONO(0.1) , DISCKY_COLOR_RED);
        discky.render();
        
        if(a.isTouchingBoundryX(discky))aSpeedX = -aSpeedX;
        if(a.isTouchingBoundryY(discky))aSpeedY = -aSpeedY;
        if(b.isTouchingBoundryX(discky))bSpeedX = -bSpeedX;
        if(b.isTouchingBoundryY(discky))bSpeedY = -bSpeedY;

        if(checkOverlap(a ,b)){
            std::swap(aSpeedX , bSpeedX);
            std::swap(aSpeedY , bSpeedY);
        }
        
        ax+=aSpeedX;
        ay+=aSpeedY;
        bx+=bSpeedX;
        by+=bSpeedY;
        
        discky.refresh();
        discky.display();
        // std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}