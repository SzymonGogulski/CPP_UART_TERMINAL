#pragma once
// C library headers
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>

// Threading libraries
#include <thread>
#include <atomic>
#include <queue>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

class SerialPortController{

    public:
        SerialPortController(const std::string& port_name, int baudrate);
        ~SerialPortController();

        std::string receiveMessage();
        bool transmitMessage(int port, const char* buffer, size_t size);

        void openSerialPort();

    private:
        std::string port_name;
        int port;
        int baudrate;
            
        void closeSerialPort();
        void configureSerialPort();
};
