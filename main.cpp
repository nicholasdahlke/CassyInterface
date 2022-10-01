#include "Cassy.h"
#include "SerialPort.h"
#include <iostream>
#include <vector>
#include <cstdint>

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

    SerialPort* serialPort;
    serialPort = new SerialPort("/dev/ttyACM0", SerialPort::SER_BAUD_9600);
    std::vector<uint8_t> vect = {49,10};
    for (int i = 0; i < 100; ++i) {
        serialPort->write_bytes(vect);
    }
    while (1);
    serialPort->disconnect();
}