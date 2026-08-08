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
    double speeda = 0.5;
    double speedb = -0.5;
    double a = 0.0;
    double b = 0.0;
    while(true){
        discky.drawCircle(COORD_NOR(0 , 0) , LEN_MONO_NOR(0.5) , DISCKY_COLOR_YELLOW);
        discky.drawTriangle(COORD_NOR(0 , 0) , COORD_NOR(1 , a) , COORD_NOR(1 , b) , DISCKY_COLOR_BLACK);
        
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