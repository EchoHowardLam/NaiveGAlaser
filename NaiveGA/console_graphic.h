#ifndef CONSOLE_GRAPHIC_H_INCLUDED
#define CONSOLE_GRAPHIC_H_INCLUDED

/*
For windows
*/

#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <conio.h>
#include <sstream>
#include <string>

using namespace std;

extern unsigned short font_color;
extern unsigned short back_color;

//extern COORD buffer_size;
extern COORD point;

extern CONSOLE_CURSOR_INFO _cursor_info;

void inline clrscr(){system("cls");}
void inline pause(){getch();}
void inline title(string input){system(("title "+input).c_str());}

void gotoxy(int x,int y); //Starts from 1
void gotoxyZERO(int x,int y); //Starts from 0

void resize_console(int col,int line);

//Color Control
void textcolor(short color);
void textbackground(short color);
void set_color(short color,short bg);
void reset_color();

//Cursor property
void cursor_visible(bool visible);
void cursor_size(DWORD cursor_size=25);

//Mouse in Console reaction
bool mouse_pos_in_console(short &x,short &y);

#endif // CONSOLE_GRAPHIC_H_INCLUDED
