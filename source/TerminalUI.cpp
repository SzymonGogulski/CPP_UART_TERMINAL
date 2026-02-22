#include "TerminalUI.h"

using namespace ftxui;

TerminalUI::TerminalUI() : screen(ScreenInteractive::Fullscreen()){}

TerminalUI::~TerminalUI() {
    screen.Exit();
}

void TerminalUI::run(){
    renderUI();
    screen.Loop(app);
}

void TerminalUI::renderUI(){
    auto left_menu = LeftMenu();
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

    app = outer;
}

Component TerminalUI::Pane(std::string title, Component child) {
    return Renderer(child, [title, child] {
        return window(text(title), child->Render() | flex);
    });
}

Component TerminalUI::ScrollableSliders(Component content, float& scroll_x, float& scroll_y){
    
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

Component TerminalUI::ScrollableReciver() {
    
    auto content = Renderer([this] {
        Elements history;
        for (const auto& msg : received_messages) {
            history.push_back(text(msg));
        }

    return vbox({std::move(history)});
    });

    return this->ScrollableSliders(content, rx_scroll_x, rx_scroll_y);
}

Component TerminalUI::LabeledDropdown(std::string title, std::vector<std::string>* entries, int* selected) {
    
    auto dropdown = Dropdown(entries, selected);

    return Renderer(dropdown, [title, dropdown] {
        return vbox({
            text(title) | bold,
            dropdown->Render(),
        });
    });
}

Component TerminalUI::LeftMenu() {

    auto layout = Container::Vertical({
        Container::Horizontal({
            LabeledDropdown("Baudrate", &entries_buadrate, &selected_baudrate),
            LabeledDropdown("Parity", &entries_parity, &selected_parity),
        }),
        Container::Horizontal({
            LabeledDropdown("Stop bits", &entries_stopbits, &selected_stopbits),
            LabeledDropdown("Data bits", &entries_databits, &selected_databits),
        }),
    });

    return layout;
}

Component TerminalUI::InputWithScrollableHistory() {

    auto content = Renderer([this] {
        Elements history;
        for (const auto& msg : transmitted_messages) {
            history.push_back(text(msg));
        }

        return vbox({std::move(history)});
    });

    auto input_component = Container::Horizontal({
        Input(&input_text, "Type message and press Enter."),
    });
    
    input_component = CatchEvent(input_component, [this](Event event) {
        if (event == Event::Return && !input_text.empty()) {
            transmitted_messages.push_back(input_text);
            received_messages.push_back(input_text);
            input_text.clear();
            return true;
        }
        return false;
    });

    return Container::Vertical({
        this->ScrollableSliders(content, tx_scroll_x, tx_scroll_y) | flex,
        Renderer([] { return separator(); }),
        input_component,
    });
}
