#include "../include/discky.h"
#include <iostream>
#include <thread>
#include <chrono>

void handleDisckyError(ERRORS error ,const std::string& msg){
    std::cerr<<msg;
}
int main(){
    iniTerminal();
    Discky discky;
    discky.setErrorHandleFunc(handleDisckyError);
    discky.setBackground(DISCKY_COLOR_BLUE);
    //if you change position , then rectangle position will be changed
    double positionX = 0.0;
    double positionY = 0.0;
    discky.drawRectangle(NOR_COORD( positionX -0.5 , positionY-0.5) , NOR_COORD( positionX +0.5 ,positionY + 0.5) , DISCKY_COLOR_RED); // DRAW A RED RECTANGLE 
    while(true){
        discky.render(); //RENDERS ALL OBJECTS ACORDING TO TERMINAL SIZE
        discky.display(); // DISPLAY THE RENDERED SCREEN
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
    endDiscky();
}