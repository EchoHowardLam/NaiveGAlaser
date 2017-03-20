#include <iostream>

#include "consoleGL.h"

#include "core.h"

using namespace std;

int main()
{
    clrscr();
    title("AI: Laser Avoidance");
    resize_console(90, 25);
    cursor_visible(false);
    Core sim;
    sim.vanillaSim();
    return 0;
}
