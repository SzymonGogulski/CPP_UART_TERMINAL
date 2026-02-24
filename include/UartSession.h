#pragma once
#include "SerialPortController.h"
#include <string>
#include <thread>
#include <atomic>
#include <queue>



class UartSession {

    public:
        std::vector<std::string> rxHistory;
        std::vector<std::string> txHistory;
        


    private:

        std::queue<std::string> txQueue;

        SerialPortController s;
        
        // Listener thread listens to Uart Rx line and writes any read message to rxHistory 
        std::thread Listener();

        // Writer waits for user to add message to txQueue, when new message arives in queue thread writes it to Uart Tx line. 
        // If write was successful thread pushes the message to txHistory. 
        std::thread Writer();
        
    
    
};
