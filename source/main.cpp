#include "TerminalUI.h"
#include "SerialPortController.h"
#include "UartSession.h"
#include <chrono>
#include <string>
#include <unistd.h>
#include <thread>

int main() {

    const std::string port = "/dev/ttyACM0";
    const std::string baudrate = "115200";

    SerialPortController s(port, baudrate);
    UartSession session(s);

    TerminalUI t(session);
    t.run();



    // UartSession test
    // UartSession session(s);
    // session.startSession();
    //
    // session.sendMessage("Hello\n");
    // std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    //
    // auto tx_history = session.getTxHistory();
    // std::cout << "Tx history: \n";
    // for (const auto& msg : tx_history) {
    //     std::cout << msg << std::endl;
    // }
    //
    // auto rx_history = session.getRxHistory();
    // std::cout << "Rx history: \n";
    // for (const auto& msg : rx_history) {
    //     std::cout << msg << std::endl;
    // }
    // session.stopSession();

    // TUI test
    // TerminalUI t;
    // t.run();

    // SerialPortController test
    // s.openSerialPort(); 
    // std::this_thread::sleep_for(std::chrono::seconds(2)); // Allow boot
    // const std::string message = "Hello\n";
    // s.transmitMessage(message.c_str(), message.size());
    // std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Allow reply
    // std::string received = s.receiveMessage();
    // std::cout << "received: " << received << std::endl;
 

    return 0;
}
