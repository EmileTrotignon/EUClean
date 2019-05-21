cmake_minimum_required(VERSION 3.14)
project(EUClean)

set(CMAKE_CXX_STANDARD 14)


#file(GLOB SRC "src/*")
include_directories(src)

add_executable(EUClean src/main_cli.cpp src/eu_clean_logic.cpp src/eu_clean_logic.h src/main_gui.cpp src/CleanConfig.cpp src/CleanConfig.h)
