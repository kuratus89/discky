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

    double r = 10;
    double speedx = 0.08;
    double speedy = 0.05;

    while(true){
        discky.drawCircle(NOR_COORD(x , y) , r , DISCKY_COLOR_GREEN);
        discky.render();
        discky.display();
        discky.refresh();

        double rx = r/((discky.getTerminalSizeX() -1)/2.0);
        double ry = r/((discky.getTerminalSizeY() -1)/2.0);
        if((x+speedx+rx>1.0)||(x+speedx-rx<-1.0))speedx = -speedx;
        else x+=speedx;
        if((y+speedy+ry>1.0)||(y+ speedy -ry<-1.0))speedy = -speedy;
        else y+=speedy;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}