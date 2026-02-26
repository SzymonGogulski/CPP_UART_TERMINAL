#include "UartSession.h"
#include "SerialPortController.h"
#include <mutex>

UartSession::UartSession(SerialPortController& s_) : s(s_){}

UartSession::~UartSession(){
    stopSession();
}

void UartSession::startSession(){
    s.openSerialPort();
    running = true;

    listenerThread = std::thread(&UartSession::listenerLoop, this);
    writerThread = std::thread(&UartSession::writerLoop, this);

} 

void UartSession::stopSession(){
    s.closeSerialPort();
    running = false;
    queueCV.notify_all();

    if (listenerThread.joinable()){
        listenerThread.join();
    }
    if (writerThread.joinable()) {
        writerThread.join();
    }

}

void UartSession::sendMessage(const std::string& message){

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

        if (!running) break;

        auto message = txQueue.front();
        txQueue.pop();
        lock.unlock();

        if (s.transmitMessage(message.c_str(), message.size())){
            std::lock_guard<std::mutex> txLock(txMutex);
            txHistory.push_back(message);
        }
    }
}

void UartSession::listenerLoop(){
    
    std::string received;

    while (running){
        received = s.receiveMessage();
        
        if (!received.empty()){
            std::lock_guard<std::mutex> lock(rxMutex);
            rxHistory.push_back(received);
        }
    } 
}

std::vector<std::string> UartSession::getRxHistory(){
    
    std::lock_guard<std::mutex> lock(rxMutex);
    return rxHistory;
}

std::vector<std::string> UartSession::getTxHistory(){
    
    std::lock_guard<std::mutex> lock(txMutex);
    return txHistory;
}
