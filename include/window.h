#pragma once

enum class OperatingSystem {
    WINDOWS,
    LINUX,
    INVALID,
};

struct TerminalInfo{
    int x;
    int y;
    OperatingSystem sysName;
};

TerminalInfo getTerminalInfo();