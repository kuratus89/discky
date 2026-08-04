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
    discky.drawPoly({COORD_NOR(0 , -1) , COORD_NOR(1 , 0) , COORD_NOR(0 , 1) , COORD_NOR(-1 , 0)} , DISCKY_COLOR_RED);
    discky.drawRectangle( COORD_NOR(-0.5+ 0.02 , -0.5 + 0.02) , COORD_NOR(0.5 -0.02 , 0.5 - 0.02) , DISCKY_COLOR_BLUE);
    discky.drawPoly({COORD_NOR(0 , -0.5+ 0.02) , COORD_NOR( 0.5 - 0.02 , 0) , COORD_NOR(0 , 0.5 - 0.02) , COORD_NOR(-0.5+ 0.02 , 0)} , DISCKY_COLOR_RED);
    while(true){
        discky.render();
        discky.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
    endDiscky();
}