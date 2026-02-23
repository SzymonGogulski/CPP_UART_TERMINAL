#include "SerialPortController.h"

SerialPortController::SerialPortController(const std::string& port_name_, int baudrate_) : port_name(port_name_), baudrate(baudrate_){}

SerialPortController::~SerialPortController() {
    closeSerialPort();
}

void SerialPortController::openSerialPort(){
    
    port = open(port_name.c_str(), O_RDWR);

    if (port < 0) {
        std::cerr << "Error opening " << port_name << ": " << strerror(errno) << std::endl;
    } else {
        std::cout << "Opened the port: " << port_name << std::endl;
    }
}

void SerialPortController::closeSerialPort(){
    close(port);

}

void SerialPortController::configureSerialPort(){
    
    struct termios tty;
    if (tcgetattr(port, &tty) != 0) {
        std::cerr << "Error getting port state: " << strerror(errno) << std::endl;
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
    }

} 

std::string SerialPortController::receiveMessage() {

    char buffer[256];

    int n = read(port, buffer, sizeof(buffer) - 1);

    if (n < 0){ 
        std::cerr << "Failed reading from port: " << strerror(errno) << std::endl;
    } else if (n > 0) {
        std::cout << "Thread read : " << n << " bytes from port. " << std::endl;
        std::cout << "Message read: " << std::string(buffer);
    }

    return std::string(buffer);
}

bool SerialPortController::transmitMessage(int port, const char* buffer, size_t size){

    if (write(port, buffer, size) < 0){
        std::cerr << "Failed writing to port: " << strerror(errno) << std::endl;
        return false;
    } else {
        return true;
    }
}


