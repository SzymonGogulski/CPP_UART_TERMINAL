add_executable(${CMAKE_PROJECT_NAME})
target_sources(${CMAKE_PROJECT_NAME} PRIVATE main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE vendor)

add_executable(example example.cpp)
target_link_libraries(example PRIVATE vendor)
