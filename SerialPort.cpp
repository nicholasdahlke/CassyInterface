//
// Created by Nicholas Dahlke on 29.09.2022.
//

#include "SerialPort.h"
#include <iostream>

int SerialPort::configure_port()
{
    const char *serial_port_name_cstr = serial_port_name.c_str();
    serial_handle = CreateFile(serial_port_name_cstr, GENERIC_READ | GENERIC_WRITE,
                               0,
                               0,
                               OPEN_EXISTING,
                               FILE_ATTRIBUTE_NORMAL,
                               0);

    if(serial_handle == INVALID_HANDLE_VALUE)
    {
        if(GetLastError() == ERROR_FILE_NOT_FOUND)
        {
            std::cerr << "Serial port doesn't exist\n";
            return -1;
        }
    }

    serial_parameters.DCBlength = sizeof(serial_parameters);

    if(!GetCommState(serial_handle, &serial_parameters))
    {
        std::cerr << "Error getting serial parameters using GetCommState \n";
        return -2;
    }

    serial_parameters.BaudRate = baud;
    serial_parameters.ByteSize = 8;
    serial_parameters.StopBits = ONESTOPBIT;
    serial_parameters.Parity = NOPARITY;

    if(!SetCommState(serial_handle, &serial_parameters))
    {
        std::cerr << "Error setting new serial parameters \n";
        return -3;
    }

    serial_timeouts.ReadIntervalTimeout = 60;
    serial_timeouts.ReadTotalTimeoutConstant = 60;
    serial_timeouts.ReadTotalTimeoutMultiplier = 15;
    serial_timeouts.WriteTotalTimeoutConstant = 60;
    serial_timeouts.WriteTotalTimeoutMultiplier = 8;

    if(!SetCommTimeouts(serial_handle, &serial_timeouts))
    {
        std::cerr << "Error setting serial port timeouts\n";
        return -4;
    }
    connected = true;

    if(!SetCommMask(serial_handle, EV_RXCHAR))
    {
        std::cerr << "Error setting com mask\n";
        return -5;
    }

    return 0;
}

SerialPort::SerialPort(std::string Port, SerialPort::BaudRate rate)
{
    if (!Port.empty())
        serial_port_name = Port;
    else
    {
        std::cerr << "Port name must not be empty \n";
    }
    baud = rate;

    configure_port();
}

int SerialPort::write_bytes(std::vector<uint8_t> data)
{
    uint8_t send_buf[data.size()];

    for(int i = 0; i < data.size(); i++)
    {
        send_buf[i] = data[i];
    }

    DWORD dwWrite = 0;

    if(!WriteFile(serial_handle, send_buf, data.size(), &dwWrite, NULL))
    {
        std::cerr << "Error writing data\n";
        return -1;
    }

    if(dwWrite != data.size())
    {
        std::cerr << "None or not all bytes written\n";
        return -2;
    }

    return dwWrite;
}

std::vector<uint8_t> SerialPort::read_bytes(int length)
{

    uint8_t read_buf[length + 1];
    DWORD dwRead = 0;
    std::vector<uint8_t> return_vector;

    while(dwRead == 0)
    {
        if(!ReadFile(serial_handle, read_buf, length, &dwRead, NULL))
        {
            std::cerr << "Error reading data \n";
            return return_vector;
        }
    }

    for(int i = 0; i < length; i++)
    {
        return_vector.push_back(read_buf[i]);
    }

    return return_vector;
}

void SerialPort::disconnect()
{
    if(!connected)
        std::cerr << "Not connected\n";
    CloseHandle(serial_handle);
}

SerialPort::~SerialPort()
{
    disconnect();
}