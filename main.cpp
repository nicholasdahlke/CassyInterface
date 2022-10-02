#include "Cassy.h"
#include "SerialPort.h"
#include <iostream>
#include <vector>
#include <cstdint>
#include "Gui.h"
#include <thread>

#ifdef _WIN32
#define PAUSE system("pause")
#endif
#ifdef linux
#define PAUSE std::cin.get()
#endif

int main()
{

    Gui* gui;
    gui = new Gui();
    gui->main_loop();

}