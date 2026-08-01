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
    discky.setBackground(DISCKY_COLOR_BLACK);
    discky.setAntiAliasing(antiAliasing::AA_LOW);
    double speeda = 0.4;
    double speedb = -0.4;
    double a = 0.0;
    double b = 0.0;
    while(true){
        discky.drawCircle(NOR_COORD(0 , 0) , NOR_LEN_MONO(0.5) , DISCKY_COLOR_YELLOW);
        discky.drawTriangle(NOR_COORD(0 , 0) , NOR_COORD(1 , a) , NOR_COORD(1 , b) , DISCKY_COLOR_BLACK);
        
        if((a<=-2)||(a>=2))speeda = -speeda;
        if((b<=-2)||(b>=2))speedb = -speedb;
        a+=speeda;
        b+=speedb;
        discky.render();
        discky.display();
        discky.refresh();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    endDiscky();
    
}