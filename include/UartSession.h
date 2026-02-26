#pragma once
#include "SerialPortController.h"
#include <condition_variable>
#include <string>
#include <thread>
#include <atomic>
#include <queue>
#include <mutex>

class UartSession {

    public:

        UartSession(SerialPortController& s_);
        ~UartSession();
        void sendMessage(const std::string& message);

        std::vector<std::string> getRxHistory();
        std::vector<std::string> getTxHistory();

        void startSession();
        void stopSession();
        
    private:
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
