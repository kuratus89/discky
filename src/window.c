#include "../include/window.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

TerminalInfo getTerminalInfo(){    
    #ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if(!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE) , &csbi))return (TerminalInfo){-1 , -1 , OS_INVALID};
    return (TerminalInfo){
        csbi.srWindow.Right - csbi.srWindow.Left ,
        csbi.srWindow.Bottom -csbi.srWindow.Top,
        OS_WINDOWS
    };
    #else
    struct winsize ws;
    if(ioctl(STDOUT_FILENO , TIOCGWINSZ , &ws)==-1)return (TerminalInfo){-1 , -1 , OS_INVALID};
    return (TerminalInfo){ws.ws_col , ws.ws_row , OS_LINUX};
    #endif
}
