#include "Cassy.h"
#include "SerialPort.h"
#include <iostream>
#include <vector>
#include <cstdint>

#ifdef _WIN32
#define PAUSE "pause"
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
    serialPort = new SerialPort("COM3", SerialPort::SER_BAUD_9600);
    std::vector<uint8_t> vect(10, 12);
    serialPort->write_bytes(vect);
    std::vector<uint8_t> answer;
    answer = serialPort->read_bytes(vect.size());

}