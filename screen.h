#pragma once

#include <windows.h>
#include <conio.h>

void setTerminalColor(WORD color);
/** 
	Set terminal color to specified color

	WORD color :
		FOREGROUND_RED
		FOREGROUND_GREEN
		FOREGROUND_BLUE
		FOREGROUND_INTENSITY
		
		BACKGROUND_RED
		BACKGROUND_GREEN
		BACKGROUND_BLUE
		BACKGROUND_INTENSITY
*/

void resetTerminalColor();
/**
	Reset the terminal color
	Actually, set these colors:
	
	FOREGROUND_BLUE
	FOREGROUND_GREEN
	FOREGROUND_RED
*/
