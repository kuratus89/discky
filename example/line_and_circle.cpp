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

    discky.drawLine(NOR_COORD(0 , -1) , NOR_COORD(0 , 1) , DISCKY_COLOR_BLACK);
    discky.drawLine(NOR_COORD(-1 , 0) , NOR_COORD(1 , 0) , DISCKY_COLOR_BLACK);
    discky.drawCircle(NOR_COORD(-0.5 , -0.5) , 8 , DISCKY_COLOR_WHITE);
    discky.drawCircle(NOR_COORD(-0.5 , 0.5) , 8 , DISCKY_COLOR_RED);
    discky.drawCircle(NOR_COORD(0.5 , -0.5) , 8 , DISCKY_COLOR_GREEN);
    discky.drawCircle(NOR_COORD(0.5 , 0.5) , 8 , DISCKY_COLOR_BLACK);
    while(true){
        discky.render();
        discky.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
}