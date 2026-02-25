#include "TerminalUI.h"
#include "SerialPortController.h"
#include "UartSession.h"
#include <string>
#include <unistd.h>
#include <thread>

int main() {

    const std::string port = "/dev/ttyACM0";
    const std::string baudrate = "115200";

    SerialPortController s(port, baudrate);
    s.openSerialPort(); 

    std::this_thread::sleep_for(std::chrono::seconds(2)); // Allow boot

    const std::string message = "Hello\n";
    s.transmitMessage(message.c_str(), message.size());

    std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Allow reply

    std::string received = s.receiveMessage();

    std::cout << "received: " << received << std::endl;

    return 0;
}
