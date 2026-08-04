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

    discky.drawCircle(COORD_NOR(-0.5 , -0.5) ,LEN_MONO_NOR(0.2)  , DISCKY_COLOR_RED);//DRAW CIRCLE AT TOP LEFT

    discky.drawLine(COORD_NOR(0 , -1) , COORD_NOR(0 , 1) , DISCKY_COLOR_BLACK); // DRAW LINES IN MIDDLE OF SCREEN
    discky.drawLine(COORD_NOR(-1 , 0) , COORD_NOR(1 , 0) , DISCKY_COLOR_BLACK); 


    // DRAW RECTANGLE
    double rectPosX = 0.5;
    double rectPosY = -0.5;
    double rectSize = 0.3;
    discky.drawRectangle(COORD_NOR(rectPosX - rectSize , rectPosY - rectSize) , COORD_NOR(rectPosX + rectSize , rectPosY + rectSize) , DISCKY_COLOR_GREEN);

    // DRAW TRIANGLE
    double triPosX = -0.5;
    double triPosY = 0.5;
    double triSize = 0.3;
    discky.drawTriangle(COORD_NOR(triPosX , triPosY - triSize) , COORD_NOR(triPosX + triSize , triPosY + triSize) , COORD_NOR(triPosX - triSize , triPosY + triSize) , DISCKY_COLOR_GREEN);

    // DRAW DIAMOND
    double diaPosX = 0.5;
    double diaPosY = 0.5;
    double diaSize = 0.3;
    discky.drawPoly({COORD_NOR(diaPosX , diaPosY - diaSize) , COORD_NOR(diaPosX + diaSize , diaPosY) , COORD_NOR(diaPosX, diaPosY + diaSize) , COORD_NOR(diaPosX - diaSize , diaPosY )} , DISCKY_COLOR_RED);

    while(true){
        discky.render(); // RENDER ALL OBJECTS
        discky.display(); // DISPLAY RENDERED SCREEN
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
    endDiscky();
}