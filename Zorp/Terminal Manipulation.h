#pragma once
#include <Windows.h>

//Creating global varibles to use to change the way text on the terminal looks and acts 
const static char* CSI = "\x1b[";
const static char* ESC = "\x1b";
const static char* TITLE = "\x1b[5;20H";
const static char* INDENT = "\x1b[5C";
const char* MAGENTA = "\x1b[95m";
const static char* YELLOW = "\x1b[93m";
const static char* RED = "\x1b[91m";
const static char* BLUE = "\x1b[94m";
const static char* WHITE = "\x1b[97m";
const static char* GREEN = "\x1b[92m";
const static char* CYAN = "\x1b[96m";
const static char* RESET_COLOR = "\x1b[0m";
const static char* SAVE_CURSOR_POS = "\x1b[s";
const static char* RESTORE_CURSOR_POS = "\x1b[u";

//To be honest I have no idea what this is doing 
//I got it from "Tutorial - Constants: Zorp part 2"
void TerminalSetup()
{
	DWORD dwMode = 0;
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);
}