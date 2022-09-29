//
// Created by Nicholas Dahlke on 29.09.2022.
//

#ifndef CASSYINTERFACE_SERIALPORT_H
#define CASSYINTERFACE_SERIALPORT_H
#include <string>
#include <vector>
#include <windows.h>

class SerialPort {
public:
    enum BaudRate
    {
        SER_BAUD_110 = 100,
        SER_BAUD_300 = 300,
        SER_BAUD_600 = 600,
        SER_BAUD_1200 = 1200,
        SER_BAUD_2400 = 2400,
        SER_BAUD_4800 = 4800,
        SER_BAUD_9600 = 9600,
        SER_BAUD_14400 = 14400,
        SER_BAUD_19200 = 19200,
        SER_BAUD_38400 = 38400,
        SER_BAUD_57600 = 57600,
        SER_BAUD_115200 = 115200,
        SER_BAUD_128000 = 128000,
        SER_BAUD_256000 = 256000
    };

    SerialPort(std::string Port, BaudRate rate);
    ~SerialPort();

    int write_bytes(std::vector<uint8_t> data);
    std::vector<uint8_t> read_bytes(int length);

    void disconnect();


private:
    HANDLE serial_handle;
    DCB serial_parameters;
    COMMTIMEOUTS serial_timeouts;

    std::string port;
    BaudRate baud;


};


#endif //CASSYINTERFACE_SERIALPORT_H
