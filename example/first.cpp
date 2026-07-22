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
        discky.drawRec(NOR_COORD(-0.5 , -0.5) , NOR_COORD(0.5 , 0.5) , DISCKY_COLOR_RED);
        discky.render();
        discky.display();
        discky.refresh();
        // std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    
}