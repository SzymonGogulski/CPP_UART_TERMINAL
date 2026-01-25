add_subdirectory(FTXUI)

add_library(vendor INTERFACE)

target_link_libraries(vendor INTERFACE 
    ftxui::screen
    ftxui::dom
    ftxui::component
)
