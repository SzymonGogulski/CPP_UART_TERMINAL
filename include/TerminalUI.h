#pragma once

#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <string>
#include <vector>
#include "UartSession.h"

class TerminalUI{

    public:
        TerminalUI(UartSession& session_);
        ~TerminalUI();
        void run();
        void renderUI();

    private:
        
        // UartSession
        UartSession& session;
        // FTXUI base components
        ftxui::Component app;
        ftxui::ScreenInteractive screen;

        // Outer split state (Left/Right)
        int lr_size = 40, lr_min = 10, lr_max = 200;
        // Inner split state (Top/Bottom) 
        int tb_size = 10, tb_min = 3, tb_max = 50;
        // Rx menu slider state 
        float rx_scroll_x = 0.1f;
        float rx_scroll_y = 0.1f;
        // Tx menu slider state
        float tx_scroll_x = 0.1f;
        float tx_scroll_y = 0.1f;
        // user input state
        std::string input_text;

        // Left pane config menus
        std::vector<std::string> entries_buadrate = {"9600", "4800", "19200", "38400", "57600", "115200", "230400", "460800", "921600"};
        std::vector<std::string> entries_parity = {"dummy"};
        std::vector<std::string> entries_stopbits = {"dummy"};
        std::vector<std::string> entries_databits = {"dummy"};
        int selected_baudrate = 0;
        int selected_parity = 0;
        int selected_stopbits = 0;
        int selected_databits = 0;

        // FTXUI custom components
        ftxui::Component Pane(std::string title, ftxui::Component child);
        ftxui::Component ScrollableSliders(ftxui::Component content, float &scroll_x, float &scroll_y);
        ftxui::Component ScrollableReciver(); 
        ftxui::Component LabeledDropdown(std::string title, std::vector<std::string>* entries, int* selected); 
        ftxui::Component LeftMenu(); 
        ftxui::Component InputWithScrollableHistory(); 
};
