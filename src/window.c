#include "../include/window.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

terminalInfo getTerminalInfo(){    
    #ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if(!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE) , &csbi))return (terminalInfo){-1 , -1 , WINDOWS};
    return (terminalInfo){
        csbi.srWindow.Right - csbi.srWindow.Left ,
        csbi.srWindow.Bottom -csbi.srWindow.Top,
        WINDOWS
    };
    #else
    struct winsize ws;
    if(ioctl(STDOUT_FILENO , TIOCGWINSZ , &ws)==-1)return (terminalInfo){-1 , -1 , LINUX};
    return (terminalInfo){ws.ws_col , ws.ws_wor , LINUX};
    #endif
}
