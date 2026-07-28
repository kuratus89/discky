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
    double pox = 0;
    double poy = 0;
    double spx = 0.05;
    double spy = 0.05;
    while(true){
        rawText& txt = discky.drawRawTxt(NOR_COORD(pox , poy) , "HELLO WORLD" , DISCKY_COLOR_RED);
        discky.render();
        if(txt.isTouchingBoundryX(discky))spx = -spx;
        if(txt.isTouchingBoundryY(discky))spy = -spy;
        pox+=spx;
        poy+=spy;
        discky.display();
        discky.refresh();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    
}