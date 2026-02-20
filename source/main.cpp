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

Component InputWithScrollableHistory() {

    return Ren
}

int main() {
    auto screen = ScreenInteractive::Fullscreen();

    // Outer split state (Left/Right)
    int lr_size = 40, lr_min = 10, lr_max = 200;

    // Inner split state (Top/Bottom) 
    int tb_size = 10, tb_min = 3, tb_max = 50;
    
    // Left pane config menus
    std::vector<int> entries_buadrate = {9600, 4800, 19200, 38400, 57600, 115200, 230400, 460800, 921600};
    std::vector<std::string> entries_parity = {};
    std::vector<std::string> entries_stop_bits = {};
    std::vector<std::string> entries_data_bits = {};
    std::vector<std::string> entries = {};

    int selected_1 = 0;
    int selected_2 = 0;
    int selected_3 = 0;
    int selected_4 = 0;

    auto layout = Container::Vertical({
        Container::Horizontal({
            LabeledDropdown("COM Port", &entries, &selected_1),
            LabeledDropdown("baudrate", &entries, &selected_2),
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

    auto left_pane_content = Renderer(layout, [&] {
        return layout->Render() | flex;
    });

    std::string input_text;
    Component input_window = Input(&input_text, "Input text");
    
    // The component tree:
    auto input_component = Container::Vertical({
        input_window,
    });
 
    // Tweak how the component tree is rendered:
    auto input_renderer = Renderer(input_component, [&] {
        return vbox({
            hbox(text(" First name : "), input_component->Render()),}) | border;
    });

    // Panes
    auto left  = Pane("Config Menu",  left_pane_content);
    auto top   = Pane("Receiver",   Renderer([] { return text("Receiver content") | center; }));
    auto bottom= Pane("Transmitter", input_renderer);


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

