#include "UartSession.h"
#include "SerialPortController.h"
#include <mutex>

UartSession::UartSession(SerialPortController& s_) : s(s_){}

UartSession::~UartSession(){}

void UartSession::startSession(){
    s.openSerialPort();
    // set running true
    // start threads

} 

void UartSession::stopSession(){
    s.closeSerialPort();
    // set running false
    // join threads
}

void UartSession::sendMessage(std::string& message){

    {    
        std::lock_guard<std::mutex> lock(queueMutex);
        txQueue.push(message);
    }

    queueCV.notify_one();
}

void UartSession::writerLoop(){

    while (running){
        std::unique_lock<std::mutex> lock(queueMutex);

        
        queueCV.wait(lock, [this] {return !txQueue.empty() || !running;});
    }
}

void UartSession::listenerLoop(){
    
    std::string received;

    while (running){
        received = s.receiveMessage();
        
        if (received != ""){
            std::lock_guard<std::mutex> lock(rxMutex);
            rxHistory.push_back(received);
        }
    } 
}
