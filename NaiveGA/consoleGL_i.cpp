#include <cstdio>

#include "consoleGL.h"

ConsoleScreen::ConsoleScreen(int Width,int Height,int LeftX,int TopY)
{
    if (Width<1){Width=1;}
    if (Height<1){Height=1;}
    if (LeftX<1){LeftX=1;}
    if (TopY<1){TopY=1;}
    colortext=COLOR_GREY;
    colorbg=COLOR_BLACK;
    bufferExist=false;
    Initialize(Width,Height,LeftX,TopY);
    return;
}

ConsoleScreen::~ConsoleScreen()
{
    DeleteBuffer();
}

void ConsoleScreen::Initialize(int Width,int Height,int LeftX,int TopY)
{
    curX=0;curY=0;left=LeftX;top=TopY;
    this->width=Width;this->height=Height;
    NewBuffer(Width,Height);
    return;
}

void ConsoleScreen::SetBufferSize(int Width,int Height)
{
    if (curX>=Width){curX=Width-1;}
    if (curY>=Height){curY=Height-1;}
    this->width=Width;this->height=Height;
    NewBuffer(Width,Height);
    return;
}

void ConsoleScreen::SetBufferPos(int LeftX,int TopY)
{
    left=LeftX;top=TopY;
    return;
}

void ConsoleScreen::UpdateScreen()
{
    int area=width*height;
    bool needupdate=false;
    bool lastupdate=false;
    int lastupdateline=0;
    int lastfontcolor=-1;
    int lastbgcolor=-1;
    for (int i=0;i<area;i++)
    {
        needupdate=false;
        if (oldBuffer[i]!=buffer[i]){oldBuffer[i]=buffer[i];needupdate=true;}
        if (oldFontColorBuffer[i]!=fontColorBuffer[i]){oldFontColorBuffer[i]=fontColorBuffer[i];needupdate=true;}
        if (oldBgColorBuffer[i]!=bgColorBuffer[i]){oldBgColorBuffer[i]=bgColorBuffer[i];needupdate=true;}
        if (needupdate)
        {
            if ((!lastupdate)||(lastupdateline!=i/width)){gotoxy(left+i%width,top+i/width);}
            if (lastfontcolor!=fontColorBuffer[i]||lastbgcolor!=bgColorBuffer[i])
            {
                lastfontcolor=fontColorBuffer[i];
                lastbgcolor=bgColorBuffer[i];
                set_color(lastfontcolor,lastbgcolor);
            }
            putchar(buffer[i]);
            lastupdate=true;
            lastupdateline=i/width;
        }else{lastupdate=false;}
    }
}

void ConsoleScreen::ClrBuffer()
{
    int area=width*height;
    for (int i=0;i<area;i++)
    {
        buffer[i]=' ';
        fontColorBuffer[i]=colortext;
        bgColorBuffer[i]=colorbg;
    }
    return;
}

void ConsoleScreen::FontColor(int Color){colortext=Color;}
void ConsoleScreen::BGColor(int Color){colorbg=Color;}
void ConsoleScreen::GotoXY(int x,int y){if (x>=1&&x<=width&&y>=1&&y<=height){curX=x-1;curY=y-1;}}

void ConsoleScreen::PushChar(char input,int x,int y,int FontColor,int BGColor)
{
    if (FontColor!=-1){colortext=FontColor;}
    if (BGColor!=-1){colorbg=BGColor;}
    this->GotoXY(x,y);
    buffer[curY*width+curX]=input;
    fontColorBuffer[curY*width+curX]=colortext;
    bgColorBuffer[curY*width+curX]=colorbg;
    if (curY<height-1)
    {
        curX++;
        curY+=(curX/width);
        curX%=width;
    }else{
        if (curX<width-1){curX++;}
    }
}

void ConsoleScreen::PushStr(string input,int x,int y,int FontColor,int BGColor)
{
    if (FontColor!=-1){colortext=FontColor;}
    if (BGColor!=-1){colorbg=BGColor;}
    this->GotoXY(x,y);
    for (unsigned int i=0;i<input.length();i++)
    {
        buffer[curY*width+curX]=input[i];
        fontColorBuffer[curY*width+curX]=colortext;
        bgColorBuffer[curY*width+curX]=colorbg;
        if (curY<height-1)
        {
            curX++;
            curY+=(curX/width);
            curX%=width;
        }else{
            if (curX<width-1){curX++;}
            else{break;} //Remainings are truncated
        }
    }
}

void ConsoleScreen::PushLine(string input,int *FontColor,int *BGColor,int x,int y)
{
    this->GotoXY(x,y);
    unsigned int cur_index=curY*width+curX;
    for (unsigned int i=0;i<input.length();i++)
    {
        buffer[cur_index]=input[i];
        fontColorBuffer[cur_index]=FontColor[i];
        bgColorBuffer[cur_index]=BGColor[i];
        cur_index++;
    }
    curX=cur_index%width;
    curY=cur_index/width;
}

void ConsoleScreen::NewBuffer(int Width,int Height)
{
    DeleteBuffer();bufferExist=true;
    int area=Width*Height;
    buffer=new char[area];
    fontColorBuffer=new int[area];
    bgColorBuffer=new int[area];
    oldBuffer=new char[area];
    oldFontColorBuffer=new int[area];
    oldBgColorBuffer=new int[area];
    for (int i=0;i<area;i++)
    {
        buffer[i]=' ';
        fontColorBuffer[i]=colortext;
        bgColorBuffer[i]=colorbg;
        oldBuffer[i]='*'; //force a redraw
        oldFontColorBuffer[i]=-1;
        oldBgColorBuffer[i]=-1;
    }
}

void ConsoleScreen::DeleteBuffer()
{
    if (bufferExist)
    {
        delete []buffer;
        delete []fontColorBuffer;
        delete []bgColorBuffer;
        delete []oldBuffer;
        delete []oldFontColorBuffer;
        delete []oldBgColorBuffer;
        bufferExist=false;
    }return;
}

