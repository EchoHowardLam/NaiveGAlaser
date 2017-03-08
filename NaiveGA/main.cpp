#include <iostream>

#include "consoleGL.h"

#include "core.h"

using namespace std;

int main()
{
    clrscr();
    title("AI: Laser Avoidance");
    Core sim;
    sim.vanillaSim();
    return 0;
}
