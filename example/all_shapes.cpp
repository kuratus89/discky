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

    discky.drawCircle(NOR_COORD(-0.5 , -0.5) ,NOR_LEN_MONO(0.2)  , DISCKY_COLOR_RED);//DRAW CIRCLE AT TOP LEFT

    discky.drawLine(NOR_COORD(0 , -1) , NOR_COORD(0 , 1) , DISCKY_COLOR_BLACK); // DRAW LINES IN MIDDLE OF SCREEN
    discky.drawLine(NOR_COORD(-1 , 0) , NOR_COORD(1 , 0) , DISCKY_COLOR_BLACK); 


    // DRAW RECTANGLE
    double rectPosX = 0.5;
    double rectPosY = -0.5;
    double rectSize = 0.3;
    discky.drawRectangle(NOR_COORD(rectPosX - rectSize , rectPosY - rectSize) , NOR_COORD(rectPosX + rectSize , rectPosY + rectSize) , DISCKY_COLOR_GREEN);

    // DRAW TRIANGLE
    double triPosX = -0.5;
    double triPosY = 0.5;
    double triSize = 0.3;
    discky.drawTriangle(NOR_COORD(triPosX , triPosY - triSize) , NOR_COORD(triPosX + triSize , triPosY + triSize) , NOR_COORD(triPosX - triSize , triPosY + triSize) , DISCKY_COLOR_GREEN);

    // DRAW DIAMOND
    double diaPosX = 0.5;
    double diaPosY = 0.5;
    double diaSize = 0.3;
    discky.drawPoly({NOR_COORD(diaPosX , diaPosY - diaSize) , NOR_COORD(diaPosX + diaSize , diaPosY) , NOR_COORD(diaPosX, diaPosY + diaSize) , NOR_COORD(diaPosX - diaSize , diaPosY )} , DISCKY_COLOR_RED);

    while(true){
        discky.render(); // RENDER ALL OBJECTS
        discky.display(); // DISPLAY RENDERED SCREEN
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
}