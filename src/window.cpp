#include "../include/window.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

TerminalInfo getTerminalInfo(){    
    // return ((TerminalInfo){10 , 20 , OS_WINDOWS});
    #ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if(!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE) , &csbi))return (TerminalInfo){-1 , -1 ,OperatingSystem::INVALID};
    int w = csbi.srWindow.Right - csbi.srWindow.Left - 1;
    int h = (csbi.srWindow.Bottom - csbi.srWindow.Top - 1) * 2;
    if(w<=0 || h<=0) return (TerminalInfo){-1,-1,OperatingSystem::INVALID};
    return (TerminalInfo){w , h ,OperatingSystem::WINDOWS};
    #else
    struct winsize ws;
    if(ioctl(STDOUT_FILENO , TIOCGWINSZ , &ws)==-1)return (TerminalInfo){-1 , -1 , OperatingSystem::INVALID};
    int w = ws.ws_col - 1;
    int h = (ws.ws_row - 1) * 2;
    if(w<=0 || h<=0) return (TerminalInfo){-1,-1,OperatingSystem::INVALID};
    return (TerminalInfo){w , h , OperatingSystem::LINUX};
    #endif
}