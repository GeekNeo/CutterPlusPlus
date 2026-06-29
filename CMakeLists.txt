cmake_minimum_required(VERSION 3.12)
project(cutterplusplus-plugin)

set(CMAKE_CXX_STANDARD 20)

find_package(Cutter REQUIRED)
find_package(Rizin REQUIRED)
set(CUTTER_INSTALL_PLUGDIR "${Cutter_USER_PLUGINDIR}" CACHE STRING "Directory to install Cutter plugin into")

set(CMAKE_AUTOMOC ON)

add_library(Cutter++ MODULE
    src/Cutter++Plugin.h
    src/Cutter++Plugin.cpp)
set_target_properties(Cutter++ PROPERTIES PREFIX "")
target_link_libraries(Cutter++ PRIVATE Cutter::Cutter Rizin::Core)

install(TARGETS Cutter++ DESTINATION "${CUTTER_INSTALL_PLUGDIR}")
