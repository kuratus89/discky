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
    std::string texts;
    out.reserve(static_cast<size_t> (terminalInfo.x)* terminalInfo.y * 20);
    moveCursor(out , 0 ,0);

    if((backBuffer.x==frontBuffer.x)&&(backBuffer.y==frontBuffer.y)&&(!forceRedraw)){
        for(int i=0; i<terminalInfo.y ; i+=2 , sy++){
            for(int j=0 ; j<terminalInfo.x ; j++){
                objColor& tf = frontBuffer.pixels[frontBuffer.x*i +j];
                objColor& tb = backBuffer.pixels[backBuffer.x*i +j];
                objColor& bf = frontBuffer.pixels[frontBuffer.x*(i+1)+ j];
                objColor& bb = backBuffer.pixels[backBuffer.x*(i+1) + j];

                if(compareObjColor(tf , tb)&&compareObjColor(bf , bb))continue;

                if((x!=j)||(y!=i)){
                    moveCursor(out , j , sy);
                    x=j;
                    y=sy;
                }
                if(!compareObjColor(bf , bgColor)){
                    colorBg(out , bf);
                    bgColor = bf;
                }
                if(compareObjColor(tf , bf)){
                    out+= " ";
                }
                else {
                    if(!compareObjColor(tf , txtColor)){
                        colorText(out , tf);
                        txtColor = tf;
                    }
                    out +="\u2580";
                }
                x++;
            }
        }
    }
    else{
        
        for(int i=0 ; i<terminalInfo.y ; i+=2 , sy++){
            for(int j=0 ; j<terminalInfo.x ; j++){

                if((x!=j)||(y!=i)){
                    moveCursor(out , j ,sy);
                    x=j;
                    y=sy;
                }
                objColor& upPixel = frontBuffer.pixels[frontBuffer.x * i + j];
                objColor& downPixel = frontBuffer.pixels[frontBuffer.x*(i+1)+j];
                
                if(!compareObjColor(downPixel , bgColor)){
                    colorBg(out , downPixel);
                    bgColor = downPixel;
                }
                
                if(compareObjColor(upPixel , downPixel)){
                    out+=" ";
                }
                else {
                    if(!compareObjColor(upPixel , txtColor)){
                        colorText(out , upPixel);
                        txtColor = upPixel;
                    }
                    out+="\u2580";
                }
                x++;

            }
        }
    }
    for(auto& txt:rawTxts){
        if((x!=txt.vertex.x)||(y!=txt.vertex.y)){
            moveCursor(texts , txt.vertex.x , txt.vertex.y);
            x = txt.vertex.x;
            y = txt.vertex.y;
        }
        if(!compareObjColor(txtColor , txt.color)){
            colorText(texts , txt.color);
            txtColor = txt.color;
        }
        texts+=txt.txt;
    }

    std::cout<<out;
    std::cout<<texts;
    std::cout.flush();
    std::swap(backBuffer , frontBuffer);
}