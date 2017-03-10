#include "console_graphic.h"

#ifdef __cplusplus
extern "C" {
#endif
BOOL WINAPI GetCurrentConsoleFont(HANDLE hConsoleOutput,BOOL bMaximumWindow,PCONSOLE_FONT_INFO lpConsoleCurrentFont);
COORD WINAPI GetConsoleFontSize(HANDLE hConsoleOutput,DWORD nFont);
#ifdef __cplusplus
}
#endif

unsigned short font_color=7;
unsigned short back_color=0;

//COORD buffer_size;
COORD point;

CONSOLE_CURSOR_INFO _cursor_info;
CONSOLE_FONT_INFO FontInfo;
RECT console_rect;
COORD font_size;
POINT mouse_pos;

void gotoxy(int x,int y)
{
    point.X=x-1,point.Y=y-1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),point);
}

void gotoxyZERO(int x,int y)
{
    point.X=x,point.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),point);
}

void resize_console(int col,int line)
{
    ostringstream tmp_command;
    tmp_command<<"mode con: cols="<<col<<" lines="<<line;
    system(tmp_command.str().c_str());
}

//Color Control
void textcolor(short color)
{
    if (color>-1 && color<16){SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),(font_color=color) | back_color);}else{exit(2);}
}
void textbackground(short color)
{
    if (color>-1 && color<16){SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),(back_color=color*16) | font_color);}else{exit(3);}
}
void set_color(short color,short bg)
{
    if (color<0 || color>15){exit(5);}
    if (bg<0 || bg>15){exit(6);}
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),(font_color=color) | (back_color=bg*16));
}
void reset_color()
{
    textbackground(0);textcolor(7);
}

//Cursor property
void cursor_visible(bool visible)
{
    _cursor_info.dwSize=25;
    _cursor_info.bVisible=visible;
    HANDLE consoleHandle=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorInfo(consoleHandle,&_cursor_info);
}
void cursor_size(DWORD cursor_size)
{
    _cursor_info.dwSize=cursor_size;
    _cursor_info.bVisible=true;
    HANDLE consoleHandle=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorInfo(consoleHandle,&_cursor_info);
}

//Mouse in Console reaction
bool mouse_pos_in_console(short &x,short &y)
{
    HANDLE consoleHandle=GetStdHandle(STD_OUTPUT_HANDLE);
    HWND console_win=GetConsoleWindow();
    if (!console_win||!GetWindowRect(console_win,&console_rect)||!GetCurrentConsoleFont(consoleHandle,0,&FontInfo)||!GetCursorPos(&mouse_pos)){return 0;}
    font_size=GetConsoleFontSize(consoleHandle,FontInfo.nFont);
    if (font_size.X==0){return 0;}
    x=((mouse_pos.x-console_rect.left-10)/font_size.X)+1;
    y=((mouse_pos.y-console_rect.top-32)/font_size.Y)+1;
    return 1;
}
