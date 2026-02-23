#include "TerminalUI.h"
#include "SerialPortController.h"

int main() {

    // TerminalUI t;
    // t.run();
    const std::string port = "/dev/ttyACM0";

    SerialPortController s(port, 115200);
    s.openSerialPort(); 

    return 0;
}

