#include <ftxui/component/component_base.hpp>
#include <string>
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

using namespace ftxui;

// Wrap a component in window with a title
Component Pane(std::string title, Component child) {
    return Renderer(child, [title, child] {
        return window(text(title), child->Render() | flex);
    });
}

// Dropdown with a title
Component LabeledDropdown(
    std::string title,
    std::vector<std::string>* entries,
    int* selected) {
    
    auto dropdown = Dropdown(entries, selected);

    return Renderer(dropdown, [title, dropdown] {
        return vbox({
            text(title) | bold,
            dropdown->Render(),
        });
    });
}

Component LeftMenu() {

    // Left pane config menus
    static std::vector<std::string> entries_buadrate = {"9600", "4800", "19200", "38400", "57600", "115200", "230400", "460800", "921600"};
    static std::vector<std::string> entries_parity = {"dummy"};
    static std::vector<std::string> entries_stop_bits = {"dummy"};
    static std::vector<std::string> entries_data_bits = {"dummy"};
    static std::vector<std::string> entries = {"dummy"};

    static int selected_1 = 0;
    static int selected_2 = 0;
    static int selected_3 = 0;
    static int selected_4 = 0;

    auto layout = Container::Vertical({
        Container::Horizontal({
            LabeledDropdown("COM Port", &entries, &selected_1),
            LabeledDropdown("baudrate", &entries_buadrate, &selected_2),
        }),
        Container::Horizontal({
            LabeledDropdown("Parity", &entries, &selected_3),
            LabeledDropdown("Stop bits", &entries, &selected_4),
        }),
        Container::Horizontal({
            LabeledDropdown("Data bits", &entries, &selected_3),
            LabeledDropdown("Connect", &entries, &selected_4),
        }),
    });

    return layout;
}

Component InputWithScrollableHistory() {

    static std::string input_text;
    static std::vector<std::string> transmitted_messages;
    
    // The component tree:
    static auto input_component = Container::Vertical({
        Input(&input_text, "Type message and press Enter."),
    });
    
    input_component = CatchEvent(input_component, [&](Event event) {
        if (event == Event::Return && !input_text.empty()) {
            transmitted_messages.push_back(input_text);
            input_text.clear();
            return true;
        }
        return false;
    });

    
    auto input_renderer = Renderer(input_component, [&] {
        
        Elements history;
        for (const auto& msg : transmitted_messages) {
            history.push_back(text(msg));
        }

        return vbox({
            vbox(std::move(history)) | flex,
            separator(),
            hbox(text("Transmit: "), input_component->Render()),}) | border;
    });
    
    return input_renderer;
}

int main() {
    auto screen = ScreenInteractive::Fullscreen();

    // Outer split state (Left/Right)
    int lr_size = 40, lr_min = 10, lr_max = 200;
    // Inner split state (Top/Bottom) 
    int tb_size = 10, tb_min = 3, tb_max = 50;
    // Left pane menu
    auto left_menu = LeftMenu();
    // Input menu with scrollable history
    auto input_with_scrollable_history = InputWithScrollableHistory();
    
    // Panes
    auto left  = Pane("Config Menu",  left_menu);
    auto top   = Pane("Receiver",   Renderer([] { return text("Receiver content") | center; }));
    auto bottom= Pane("Transmitter", input_with_scrollable_history);


    // Right split (Top/Bottom)
    auto right_split = ResizableSplit({
        .main = top,
        .back = bottom,
        .direction = Direction::Up,
        .main_size = &tb_size,
        .min = &tb_min,
        .max = &tb_max,
    });

    // Outer split (Left/Right)
    auto outer = ResizableSplit({
        .main = left,
        .back = right_split,
        .direction = Direction::Left,
        .main_size = &lr_size,
        .min = &lr_min,
        .max = &lr_max,
    });

    auto app = Renderer(outer, [&] {
        return outer->Render();
    });

    screen.Loop(app);
}

