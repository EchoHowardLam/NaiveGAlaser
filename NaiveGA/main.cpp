#include <iostream>

#include "consoleGL.h"

#include "core.h"

using namespace std;

int main()
{
    clrscr();
    title("AI: Laser Avoidance");
    cursor_visible(false);
    Core sim;
    sim.vanillaSim();
    return 0;
}
