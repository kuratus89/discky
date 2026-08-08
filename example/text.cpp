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
    discky.setForceRedraw();
    double pox = 0;
    double poy = 0;
    double spx = 0.05;
    double spy = 0.05;
    while(true){
        objId txt = discky.drawRawTxt(COORD_TXT_NOR(pox , poy) , "HELLO WORLD" , DISCKY_COLOR_RED);
        discky.render();
        if(discky.isTextTouchingTop(txt))spy = std::abs(spy);
        if(discky.isTextTouchingBottom(txt))spy = -std::abs(spy);
        if(discky.isTextTouchingLeft(txt))spx = std::abs(spx);
        if(discky.isTextTouchingRight(txt))spx = -std::abs(spx);
        pox+=spx;
        poy+=spy;
        discky.display();
        discky.refresh();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    endDiscky();
}