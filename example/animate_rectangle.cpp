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
    double x = -0.5;
    double y = 0.0;

    double size = 0.25;
    double speedx = 0.08;
    double speedy = 0.05;

    while(true){
        objId rect = discky.drawRectangle(COORD_NOR(x - size , y- size) , COORD_NOR(x+size , y+size) , DISCKY_COLOR_GREEN);
        discky.render();
        discky.display();
        if(discky.isObjectTouchingTop(rect))speedy = std::abs(speedy);
        if(discky.isObjectTouchingBottom(rect))speedy = -std::abs(speedy);
        if(discky.isObjectTouchingLeft(rect))speedx = std::abs(speedx);
        if(discky.isObjectTouchingRight(rect))speedx = -std::abs(speedx);
        x+=speedx;
        y+=speedy;
        
        discky.refresh();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    endDiscky();
}