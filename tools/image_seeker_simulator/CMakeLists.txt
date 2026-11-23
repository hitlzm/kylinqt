cmake_minimum_required(VERSION 3.16)

project(image_seeker_simulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 5.12 COMPONENTS Core SerialPort REQUIRED)

add_executable(image_seeker_simulator main.cpp)
target_link_libraries(image_seeker_simulator PRIVATE
    Qt5::Core
    Qt5::SerialPort
)

if(WIN32)
    set_target_properties(image_seeker_simulator PROPERTIES
        WIN32_EXECUTABLE FALSE
    )
endif()
