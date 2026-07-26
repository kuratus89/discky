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
    while(true){
        int r = std::min(discky.getTerminalSizeX() , discky.getTerminalSizeY());
        r *= (0.25 - 0.04);
        discky.drawCircle(NOR_COORD(-0.5 , -0.5) , r , DISCKY_COLOR_RED);
        discky.drawLine(NOR_COORD(0 , -1) , NOR_COORD(0 , 1) , DISCKY_COLOR_BLACK);
        discky.drawLine(NOR_COORD(-1 , 0) , NOR_COORD(1 , 0) , DISCKY_COLOR_BLACK);
        discky.drawRectangle(NOR_COORD(0.5 - 0.3 , -0.5 - 0.3) , NOR_COORD(0.5 + 0.3 , -0.5 + 0.3) , DISCKY_COLOR_GREEN , 0.5 );
        discky.drawTriangle(NOR_COORD(-0.5  , 0.5 - 0.3) , NOR_COORD(-0.5 -0.3 , 0.5 + 0.3) , NOR_COORD(-0.5+0.3 , 0.5 + 0.3) , DISCKY_COLOR_GREEN);
        discky.drawPoly({NOR_COORD(0.5 , 0.5 - 0.3) , NOR_COORD(0.5 + 0.3 , 0.5) , NOR_COORD(0.5 , 0.5 + 0.3) , NOR_COORD(0.5 - 0.3, 0.5)} , DISCKY_COLOR_RED);
        discky.render();
        discky.display();
        discky.refresh();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}