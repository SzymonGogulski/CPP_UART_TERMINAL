// C library headers
#include <stdio.h>
#include <string.h>
#include <iostream>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

int openSerialPort(const char* port){
    
    int serial_port = open(port, O_RDWR);

    if (serial_port < 0) {
        std::cerr << "Error opening " << port << ": " << strerror(errno) << std::endl;
        return -1;
    } else {
        std::cout << "Opened the port: " << port << std::endl;
    }
    
    return serial_port;
}

bool configureSerialPort(int port, int baudrate){
    
    struct termios tty;
    if (tcgetattr(port, &tty) != 0) {
        std::cerr << "Error getting port state: " << strerror(errno) << std::endl;
        return false;
    }
    
    cfsetispeed(&tty, baudrate);
    cfsetospeed(&tty, baudrate);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
    tty.c_iflag &= ~IGNBRK;
    tty.c_lflag = 0;
    tty.c_oflag = 0;
    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = 5;
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~(PARENB | PARODD);
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    if (tcsetattr(port, TCSANOW, &tty) != 0){
        std::cerr << "Error writing configureSerialPort: " << strerror(errno) << std::endl;
        return false;
    }

    return true;
} 

void closeSerialPort(int port){
    close(port);
}

int arg2Baudrate(const char* arg){

    int baudrate = std::stoi(arg);

    switch(baudrate){
        case 115200: return B115200;
        default:
            std::cerr << "Error setting baudrate: " << baudrate << std::endl;
            return -1;
    }
}

void writeToSerialPort(int port, const char* buffer, size_t size){

    if (write(port, buffer, size) < 0){
        std::cerr << "Failed writing to port: " << strerror(errno) << std::endl;
    }
}

int readFromSerialPort(int port, char* buffer, size_t size){

    int n = read(port, buffer, size);
    if (n < 0){ 
        std::cerr << "Failed reading from port: " << strerror(errno) << std::endl;
    } else {
        std::cout << "Read : " << n << " bytes from port. " << std::endl;
        std::cout << "Message read: " << std::string(buffer);
    }
    return n;
}

int main(int argc, char* argv[]) {
    
    const char* port_arg = argv[1];
    int baudrate_arg = arg2Baudrate(argv[2]);
    std::string msg = std::string(argv[3]) + "\n"; 
    char buffer[100];
    
    std::cout << "Port: " << argv[1] << " | baudrate: " << argv[2] << " | message: " << argv[3] << std::endl;
    
    int port = openSerialPort(port_arg);

    if (!configureSerialPort(port, baudrate_arg)){
        closeSerialPort(port);
        return EXIT_FAILURE;
    }

    writeToSerialPort(port, msg.c_str(), msg.size());
    readFromSerialPort(port, buffer, sizeof(buffer));
    closeSerialPort(port);

    return 0;
}

