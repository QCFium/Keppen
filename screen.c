#include "screen.h"

inline void setTerminalColor(WORD color) {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, color);
}

inline void resetTerminalColor() {
    setTerminalColor(FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
}
