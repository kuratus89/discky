#pragma once

typedef enum{
    OS_WINDOWS,
    OS_LINUX,
    OS_INVALID,
    
}operatingSystem;

typedef struct{
    int x;
    int y;
    operatingSystem sysName;
}TerminalInfo;

TerminalInfo getTerminalInfo();