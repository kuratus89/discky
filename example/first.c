#include "../include/discky.h"
int main(){
    Discky discky;
    iniDiscky(&discky);
    setDisckyBackground(&discky , DISCKY_COLOR_BLUE);
    
    disckyDrawRec(&discky ,NOR_COORD(-1 , -1) , NOR_COORD(0,0) , DISCKY_COLOR_BLUE );
    renderDiscky(&discky);
    displayDiscky(&discky);
}