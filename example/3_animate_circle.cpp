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
    discky.setAntiAliasing(antiAliasing::AA_HIGH);
    double x = -0.5;
    double y = 0.0;

    double r = 10;
    double speedx = 0.08;
    double speedy = 0.05;

    while(true){
        objects& cir = discky.drawCircle(NOR_COORD(x , y) , NOR_COORD(0.2 , 0.2) , DISCKY_COLOR_GREEN);
        discky.render();
        discky.display();
        if(cir.isTouchingBoundryX(discky))speedx = -speedx;
        x+=speedx;
        if(cir.isTouchingBoundryY(discky))speedy = -speedy;
        y+=speedy;
        discky.refresh();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}