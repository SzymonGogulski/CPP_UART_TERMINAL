#include <ftxui/component/component_base.hpp>
#include <ftxui/screen/string.hpp>
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

Component ScrollableSliders(Component content, float &scroll_x, float &scroll_y){
    
    SliderOption<float> option_x;
    option_x.value = &scroll_x;
    option_x.min = 0.f;
    option_x.max = 1.f;
    option_x.increment = 0.1f;
    option_x.direction = Direction::Right;
    option_x.color_active = Color::Blue;
    option_x.color_inactive = Color::BlueLight;
    auto scrollbar_x = Slider(option_x);

    SliderOption<float> option_y;
    option_y.value = &scroll_y;
    option_y.min = 0.f;
    option_y.max = 1.f;
    option_y.increment = 0.1f;
    option_y.direction = Direction::Down;
    option_y.color_active = Color::Yellow;
    option_y.color_inactive = Color::YellowLight;
    auto scrollbar_y = Slider(option_y);
    
    auto scrollable_content = Renderer(content, [&, content] {
        return content->Render() | focusPositionRelative(scroll_x, scroll_y) | frame | flex;
    });

    return Container::Vertical({
        Container::Horizontal({
            scrollable_content,
            scrollbar_y,
        }) | flex,
        Container::Horizontal({
            scrollbar_x,
            Renderer([] { return text(L"x"); }),
        }),
    });
}

static std::vector<std::string> received_messages;
static std::vector<std::string> transmitted_messages;

Component ScrollableReciver() {
    
    static float scroll_x = 0.1f;
    static float scroll_y = 0.1f;

    auto content = Renderer([=] {
        Elements history;
        for (const auto& msg : received_messages) {
            history.push_back(text(msg));
        }

    return vbox({std::move(history)});
    });

    return ScrollableSliders(content, scroll_x, scroll_y);
}

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
    
    static float scroll_x = 0.1f;
    static float scroll_y = 0.1f;
    static std::string input_text;

    auto content = Renderer([=] {
        Elements history;
        for (const auto& msg : transmitted_messages) {
            history.push_back(text(msg));
        }

        return vbox({std::move(history)});
    });

    auto input_component = Container::Horizontal({
        Input(&input_text, "Type message and press Enter."),
    });
    
    input_component = CatchEvent(input_component, [&](Event event) {
        if (event == Event::Return && !input_text.empty()) {
            transmitted_messages.push_back(input_text);
            received_messages.push_back(input_text);
            input_text.clear();
            return true;
        }
        return false;
    });

    return Container::Vertical({
        ScrollableSliders(content, scroll_x, scroll_y) | flex,
        Renderer([] { return separator(); }),
        input_component,
    });
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
    auto scrollable_receiver = ScrollableReciver();
    
    // Panes
    auto left  = Pane("Config Menu",  left_menu);
    auto top   = Pane("Receiver",   scrollable_receiver);
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

