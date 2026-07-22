#include "../include/screen.h"
#include <iostream>
#include <string>





static void moveCursor(std::string& out , int x , int y){
    out+="\033[" + std::to_string(y+1)+";" + std::to_string(x+1) + "H";
}

static void colorText(std::string& out , const objColor& color){
    out+= "\033[38;2;" + std::to_string(color.r) + ";" + std::to_string(color.g) + ";" + std::to_string(color.b) + "m";
}

static void colorBg(std::string& out , const objColor& color){
    out += "\033[48;2;" + std::to_string(color.r) + ";" + std::to_string(color.g) + ";" + std::to_string(color.b) + "m";
}

static bool compareObjColor(objColor& x , objColor& y){
    if(x.r !=y.r)return 0;
    if(x.g !=y.g)return 0;
    if(x.b != y.b)return 0;
    return 1;
}


void Discky::display(){
    // will add optimized printer later

    // renderNewBg(discky , discky->terminalInfo.x , discky->terminalInfo.y , discky->frontBuffer);
    objColor txtColor = {-1,-1,-1};
    objColor bgColor ={-1,-1,-1};
    int x=0;
    int y=0;
    int sy=0;
    
    std::string out;
    out.reserve(static_cast<size_t> (terminalInfo.x)* terminalInfo.y * 20);
    moveCursor(out , 0 ,0);
    for(int i=0 ; i<terminalInfo.y ; i+=2 , sy++){
        for(int j=0 ; j<terminalInfo.x ; j++){

            if((x!=j)||(y!=i)){
                moveCursor(out , j ,sy);
                x=j;
                y=sy;
            }
            objColor& upPixel = frontBuffer.pixels[frontBuffer.x * i + j];
            objColor& downPixel = frontBuffer.pixels[frontBuffer.x*(i+1)+j];
            if(!compareObjColor(upPixel , txtColor)){
                colorText(out , upPixel);
                txtColor = upPixel;
            }
            if(!compareObjColor(downPixel , bgColor)){
                colorBg(out , downPixel);
                bgColor = downPixel;
            }
            out+="\u2580";
            x++;
        }
    }

    std::cout<<out;
    std::cout.flush();
    std::swap(backBuffer , frontBuffer);
}