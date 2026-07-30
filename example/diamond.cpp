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
    discky.setAntiAliasing(antiAliasing::AA_LOW);
    discky.drawPoly({NOR_COORD(0 , -1) , NOR_COORD(1 , 0) , NOR_COORD(0 , 1) , NOR_COORD(-1 , 0)} , DISCKY_COLOR_RED);
    discky.drawRectangle( NOR_COORD(-0.5+ 0.02 , -0.5 + 0.02) , NOR_COORD(0.5 -0.02 , 0.5 - 0.02) , DISCKY_COLOR_BLUE);
    discky.drawPoly({NOR_COORD(0 , -0.5+ 0.02) , NOR_COORD( 0.5 - 0.02 , 0) , NOR_COORD(0 , 0.5 - 0.02) , NOR_COORD(-0.5+ 0.02 , 0)} , DISCKY_COLOR_RED);
    while(true){
        discky.render();
        discky.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
    
}