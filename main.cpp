#include "Cassy.h"
#include <iostream>

#ifdef _WIN32
#define PAUSE "pause"
#endif

int main()
{
    Cassy* cassy;
    cassy = new Cassy();
    cassy->connect();
    cassy->set_relay(cassy->relays[0], true);
    cassy->set_relay(cassy->relays[1], true);
    std::cout << std::to_string(cassy->read_voltage(cassy->voltage_channels[0], cassy->v4)) << std::endl;
    std::cout << std::to_string(cassy->read_voltage(cassy->voltage_channels[1], cassy->v4)) << std::endl;
    std::cout << std::to_string(cassy->read_voltage(cassy->voltage_channels[2], cassy->v4)) << std::endl;
    system(PAUSE);
    cassy->set_relay(cassy->relays[0], false);
    cassy->set_relay(cassy->relays[1], false);

    cassy->disconnect();
}