# UI Library
add_library(terminal_ui TerminalUI.cpp)

target_include_directories(terminal_ui
    PUBLIC 
        "${PROJECT_SOURCE_DIR}/include"
)

target_link_libraries(terminal_ui 
    PUBLIC 
        ftxui::screen  # Or whatever the ftxui target name is in your vendor folder
        ftxui::dom
        ftxui::component
)

# Main Executable
add_executable(${CMAKE_PROJECT_NAME} main.cpp)

# Linking terminal_ui automatically pulls in the /include directory 
# because we marked it as PUBLIC above.
target_link_libraries(${CMAKE_PROJECT_NAME} 
    PRIVATE 
        terminal_ui 
        vendor
)

target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
