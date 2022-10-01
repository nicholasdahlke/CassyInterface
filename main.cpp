#include "Cassy.h"
#include "SerialPort.h"
#include <iostream>
#include <vector>
#include <cstdint>
#include "Gui.h"

#ifdef _WIN32
#define PAUSE system("pause")
#endif
#ifdef linux
#define PAUSE std::cin.get()
#endif

int main()
{
    /*Cassy* cassy;
    cassy = new Cassy();
    cassy->connect();
    cassy->set_relay(cassy->relays[0], true);
    cassy->set_relay(cassy->relays[1], true);
    system(PAUSE);
    cassy->set_relay(cassy->relays[0], false);
    cassy->set_relay(cassy->relays[1], false);
    cassy->disconnect();
     */
    Gui* gui;
    gui = new Gui();
    gui->main_loop();

}