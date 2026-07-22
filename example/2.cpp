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

    double size = 0.5;
    double speedx = 0.05;
    double speedy = 0.05;

    while(true){
        discky.drawRec(NOR_COORD(x - size , y- size) , NOR_COORD(x+size , y+size) , DISCKY_COLOR_GREEN);
        discky.render();
        discky.display();
        discky.refresh();
        
        if((x+size+speedx>1.0)||(x-size+ speedx<-1.0))speedx = -speedx;
        else x+=speedx;
        if((y+ size+speedy>1.0)||(y - size + speedy < -1.0))speedy = - speedy;
        else y+=speedy;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}