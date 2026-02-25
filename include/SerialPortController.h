#pragma once
#include <stdio.h>
#include <string.h>
#include <iostream>

#include <fcntl.h> // Contains file controls like O_RDWR
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()


class SerialPortController{

    public:
        SerialPortController(const std::string& port_name, const std::string& baudrate);
        ~SerialPortController();

        std::string receiveMessage();
        bool transmitMessage(const char* buffer, size_t size);

        void openSerialPort();            
        void closeSerialPort();

    private:
        std::string port_name;
        int port;
        int baudrate;

        int arg2Baud(const std::string& arg);
        void configureSerialPort();
};
