cmake_minimum_required(VERSION 3.14)
project(EUClean)

set(CMAKE_CXX_STANDARD 14)


#file(GLOB SRC "src/*")
include_directories(src)

file(GLOB LOGIC src/CleanConfig.cpp src/CleanConfig.h
        src/eu_clean_logic.cpp src/eu_clean_logic.h)


add_executable(EUClean_CLI src/main_cli.cpp src/CleanConfig.cpp src/CleanConfig.h
        src/eu_clean_logic.cpp src/eu_clean_logic.h)

find_package(Qt5Widgets)

message("Qt5Widgets found: ${Qt5Widgets_FOUND}")

if (Qt5Widgets_FOUND)

    # Instruct CMake to run moc automatically when needed
    set(CMAKE_AUTOMOC ON)

    find_package(Qt5Widgets CONFIG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

    add_executable(EUClean_GUI
            src/main_gui.cpp
            src/CleanConfig.cpp src/CleanConfig.h
            src/eu_clean_logic.cpp src/eu_clean_logic.h
            src/QFileSelect.cpp src/QFileSelect.h
            src/EuCleanCentralWidget.cpp src/EuCleanCentralWidget.h)
    target_link_libraries(EUClean_GUI Qt5::Widgets)
endif (Qt5Widgets_FOUND)
