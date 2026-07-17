#pragma once

typedef enum{
    WINDOWS,
    LINUX
}operatingSystem;

typedef struct{
    int x;
    int y;
    operatingSystem sysName;
}terminalInfo;

terminalInfo getTerminalInfo();