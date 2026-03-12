#pragma once
#include "SerialPortController.h"
#include <condition_variable>
#include <string>
#include <thread>
#include <atomic>
#include <queue>
#include <mutex>
#include <functional>

class UartSession {

    public:

        UartSession(SerialPortController& s_);
        ~UartSession();
        void sendMessage(const std::string& message);

        std::vector<std::string> getRxHistory();
        std::vector<std::string> getTxHistory();

        void startSession();
        void setReceiveCallback(std::function<void()> cb);
        void stopSession();
        
    private:

        std::function<void()> onReceive;
        
        std::atomic<bool> running{false};

        void listenerLoop();
        void writerLoop();

        std::vector<std::string> rxHistory;
        std::vector<std::string> txHistory;
        std::queue<std::string> txQueue;
        
        std::mutex rxMutex;
        std::mutex txMutex;
        std::condition_variable queueCV;
        std::mutex queueMutex;

        SerialPortController& s;

        std::thread listenerThread;
        std::thread writerThread;
        
    
    
};
