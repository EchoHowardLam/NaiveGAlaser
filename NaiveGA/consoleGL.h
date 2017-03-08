#ifndef CONSOLEGL_H_INCLUDED
#define CONSOLEGL_H_INCLUDED

#include <string>

#include "console_graphic.h"

#define COLOR_BLACK 0
#define COLOR_DARK_BLUE 1
#define COLOR_DARK_GREEN 2
#define COLOR_DARK_CYAN 3
#define COLOR_DARK_RED 4
#define COLOR_DARK_PURPLE 5
#define COLOR_DARK_YELLOW 6
#define COLOR_GREY 7
#define COLOR_DARK_GREY 8
#define COLOR_BLUE 9
#define COLOR_GREEN 10
#define COLOR_CYAN 11
#define COLOR_RED 12
#define COLOR_PURPLE 13
#define COLOR_YELLOW 14
#define COLOR_WHITE 15

using namespace std;

class ConsoleScreen
{
public:
    ConsoleScreen(int Width=1,int Height=1,int LeftX=1,int TopY=1);
    ~ConsoleScreen();
    void Initialize(int Width,int Height,int LeftX,int TopY);
    void SetBufferSize(int Width,int Height);
    void SetBufferPos(int LeftX,int TopY);
    //Implementation
    void UpdateScreen();
    void ClrBuffer();
    void FontColor(int Color);
    void BGColor(int Color);
    void GotoXY(int x,int y); //Starts from 1
    void PushChar(char input,int x=-1,int y=-1,int FontColor=-1,int BGColor=-1); //Starts from 1
    void PushStr(string input,int x=-1,int y=-1,int FontColor=-1,int BGColor=-1); //Starts from 1
    void PushLine(string input,int *FontColor,int *BGColor,int x=-1,int y=-1); //Starts from 1
private:
    void NewBuffer(int Width,int Height);
    void DeleteBuffer(); //Assume there must be a buffer except when destructed
    bool bufferExist;
    char *buffer;
    int *fontColorBuffer;
    int *bgColorBuffer;
    char *oldBuffer;
    int *oldFontColorBuffer;
    int *oldBgColorBuffer;
    int curX; //Starts from 0
    int curY; //Starts from 0
    int width;
    int height;
    int left; //Starts from 1
    int top; //Starts from 1
    int colortext;
    int colorbg;
};

#endif // CONSOLEGL_H_INCLUDED
