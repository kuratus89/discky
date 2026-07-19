#include "../include/screen.h"

Vec intToWString(Discky* discky , int n ){
    Vec wstring;
    iniVec(&wstring , sizeof(wchar_t));
    *(void**)pushVec(discky , &discky->garbge) = wstring.vector;
    int i=0;
    if(!n){
        *(wchar_t*)pushVec(discky , &wstring) = L'0';
        // *(wchar_t*)pushVec(discky , &wstring) = L'\0';
        return wstring;
    }

    while(n>0){
        *(wchar_t*)pushVec(discky , &wstring) = (wchar_t)((n%10) + L'0');
        i++;
        n/=10;
    }
    // *(wchar_t*)pushVec(discky , &wstring) = L'\0';

    for(int x=0 ; x<i/2 ; x++){
        wchar_t temp = *(wchar_t*)getVecElement(discky , &wstring , x);
        *(wchar_t*)getVecElement(discky ,&wstring , x) = *(wchar_t*)getVecElement(discky , &wstring , i-x -1);
        *(wchar_t*)getVecElement(discky , &wstring , i-x-1) = temp;
    }
    return wstring;
}

void pushWStringToDisplay(Discky* discky , Vec* display , Vec wstring){
    for(int i=0 ; i!=wstring.count ; i++){
        *(wchar_t*)pushVec(discky , display) =*(wchar_t*) getVecElement(discky , &wstring , i);
    }
}

void moveCursor(Discky* discky ,Vec* display , int x , int y){
    //\033[x;yH
    
    *(wchar_t*)pushVec(discky , display) = L'\033';
    *(wchar_t*)pushVec(discky , display) = L'[';
    // *(wchar_t*)pushVec(discky , display) = L'0' + x;
    pushWStringToDisplay(discky , display  , intToWString(discky , x));
    *(wchar_t*)pushVec(discky , display) = L';';
    // *(wchar_t*)pushVec(discky , display) = L'0' +y;
    pushWStringToDisplay(discky , display , intToWString(discky , y));
    *(wchar_t*)pushVec(discky , display) = L'H';
}

void colorText(Discky* discky , Vec* display , objColor color){
    //\033[38;2;r;g;bm
    *(wchar_t*)pushVec(discky , display) = L'\003';
    *(wchar_t*)pushVec(discky , display) = L'[';
    *(wchar_t*)pushVec(discky , display) = L'3';
    *(wchar_t*)pushVec(discky , display) = L'8';
    *(wchar_t*)pushVec(display , display) = L';';
    *(wchar_t*)pushVec(display , display) = L'2';
    *(wchar_t*)pushVec(discky , display) = L';';
    pushWStringToDisplay(discky , display , intToWString(discky , color.r));
    *(wchar_t*)pushVec(discky , display) = L';';
    pushWStringToDisplay(discky , display , intToWString(discky , color.g));
    *(wchar_t*)pushVec(discky , display) = L';';
    pushWStringToDisplay(discky , display , intToWString(discky , color.b));
    *(wchar_t*)pushVec(discky , display) = L'm';
}

void colorBg(Discky* discky , Vec* display , objColor color){
    //\033[48;2;r;g;bm
    *(wchar_t*)pushVec(discky , display) = L'\003';
    *(wchar_t*)pushVec(discky , display) = L'[';
    *(wchar_t*)pushVec(discky , display) = L'4';
    *(wchar_t*)pushVec(discky , display) = L'8';
    *(wchar_t*)pushVec(discky , display) = L';';
    *(wchar_t*)pushVec(discky , display) = L'2';
    *(wchar_t*)pushVec(discky , display) = L';';
    pushWStringToDisplay(discky , display , intToWString(discky , color.r));
    *(wchar_t*)pushVec(discky ,display) = L';';
    pushWStringToDisplay(discky , display , intToWString(discky , color.g));
    *(wchar_t*)pushVec(discky , display) = L';';
    pushWStringToDisplay(discky , display , intToWString(discky , color.b));
    *(wchar_t*)pushVec(discky , display) = L'm';
}

bool compareObjColor(objColor* x , objColor* y){
    if(x->r !=y->r)return 0;
    if(x->g !=y->g)return 0;
    if(x->b != y->b)return 0;
    return 1;
}
void renderNewBg(Discky* discky , int x , int y , Screen* screen){
    screen->x=x;
    screen->y=y;
    int size = x*y;
    resizeVec(discky , &screen->pixels , size);
    for(int i=0 ; i!=size ; i++)*(objColor*)getVecElement(discky , &screen->pixels , i) = discky->bgColor;
}

void displayDiscky(Discky* discky){
    // will add optimized printer later

    renderNewBg(discky , discky->terminalInfo.x , discky->terminalInfo.y , discky->frontBuffer);
    objColor txtColor = (objColor){-1,-1,-1};
    objColor bgColor = (objColor){-1,-1,-1};
    int x=0;
    int y=0;
    
    Vec display;
    iniVec(&display , sizeof(wchar_t));
    moveCursor(discky , &display , 0 ,0);
    for(int i=0 ; i<discky->terminalInfo.y ; i+=2){
        for(int j=0 ; j<discky->terminalInfo.x ; j++){

            if((x!=j)||(y!=i)){
                moveCursor(discky, &display , j , i);
                x=j;
                y=i;
            }
            objColor* upPixel =getVecElement(discky , &discky->frontBuffer->pixels ,( discky->frontBuffer->x * i)+ j);
            objColor* downPixel = getVecElement(discky , &discky->frontBuffer->pixels , (discky->frontBuffer->x * (i+1))+j);
            if(!compareObjColor(upPixel , &txtColor))colorText(discky , &display , *upPixel);
            if(!compareObjColor(downPixel , &bgColor))colorBg(discky , &display , *downPixel);
            *(wchar_t*)pushVec(discky , &display) = L'▀';
            x++;
        }
    }

    *(wchar_t*)pushVec(discky , &display) = L'\0';
    wprintf(L"%ls\n" , display.vector);

    SWAP(discky->backBuffer , discky->frontBuffer);
}