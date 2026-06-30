cmake_minimum_required(VERSION 3.12)
project(cutterplusplus-plugin)

set(CMAKE_CXX_STANDARD 20)

find_package(Cutter REQUIRED)
find_package(Rizin REQUIRED)
set(CUTTER_INSTALL_PLUGDIR "${Cutter_USER_PLUGINDIR}" CACHE STRING "Directory to install Cutter plugin into")

set(CMAKE_AUTOMOC ON)

# FeatherPad for C++ code highlighter
set(FeatherPad third/FeatherPad/featherpad)

add_library(Cutter++ MODULE
    src/Cutter++Plugin.h
    src/Cutter++Plugin.cpp
    src/SourceEdit.h
    src/SourceEdit.cpp

    ${FeatherPad}/config.cpp
    ${FeatherPad}/textedit.cpp
    ${FeatherPad}/vscrollbar.cpp
    ${FeatherPad}/highlighter/highlighter.cpp
    ${FeatherPad}/highlighter/highlighter-cmake.cpp
    ${FeatherPad}/highlighter/highlighter-css.cpp
    ${FeatherPad}/highlighter/highlighter-fountain.cpp
    ${FeatherPad}/highlighter/highlighter-java.cpp
    ${FeatherPad}/highlighter/highlighter-json.cpp
    ${FeatherPad}/highlighter/highlighter-html.cpp
    ${FeatherPad}/highlighter/highlighter-lua.cpp
    ${FeatherPad}/highlighter/highlighter-markdown.cpp
    ${FeatherPad}/highlighter/highlighter-pascal.cpp
    ${FeatherPad}/highlighter/highlighter-patterns.cpp
    ${FeatherPad}/highlighter/highlighter-perl-regex.cpp
    ${FeatherPad}/highlighter/highlighter-regex.cpp
    ${FeatherPad}/highlighter/highlighter-rest.cpp
    ${FeatherPad}/highlighter/highlighter-ruby.cpp
    ${FeatherPad}/highlighter/highlighter-rust.cpp
    ${FeatherPad}/highlighter/highlighter-sh.cpp
    ${FeatherPad}/highlighter/highlighter-tcl.cpp
    ${FeatherPad}/highlighter/highlighter-toml.cpp
    ${FeatherPad}/highlighter/highlighter-xml.cpp
    ${FeatherPad}/highlighter/highlighter-yaml.cpp
)
set_target_properties(Cutter++ PROPERTIES PREFIX "")
target_include_directories(Cutter++ PRIVATE 
    third/FeatherPad
)
target_link_libraries(Cutter++ PRIVATE Cutter::Cutter Rizin::Core)

install(TARGETS Cutter++ DESTINATION "${CUTTER_INSTALL_PLUGDIR}")
