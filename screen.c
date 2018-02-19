#include "screen.h"

inline void setTextColor(WORD color) {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, color);
}

inline void resetTextColor() {
    setTextColor(FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
}
