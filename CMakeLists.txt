cmake_minimum_required(VERSION 3.12)
project(cutterplusplus-plugin)

set(CMAKE_CXX_STANDARD 20)

find_package(Cutter REQUIRED)
find_package(Rizin REQUIRED)
set(CUTTER_INSTALL_PLUGDIR "${Cutter_USER_PLUGINDIR}" CACHE STRING "Directory to install Cutter plugin into")

set(CMAKE_AUTOMOC ON)

# Use $ORIGIN to point to the directories relative to the binary
if(UNIX AND NOT APPLE)
    set(CMAKE_SKIP_BUILD_RPATH FALSE)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)
    set(CMAKE_INSTALL_RPATH "$ORIGIN:$ORIGIN/../lib")
endif()

# FeatherPad for C++ code highlighter
set(FeatherPad third/FeatherPad/featherpad)

add_library(Cutter++ MODULE
    include/Cutter++.h
    src/Cutter++.cpp
    src/Cutter++Plugin.h
    src/Cutter++Plugin.cpp
    src/SourceEdit.h
    src/SourceEdit.cpp
    src/ICPPExec.h
    src/ICPPExec.cpp
    src/Utilities.h
    src/Utilities.cpp
    src/AboutDialog.h
    src/AboutDialog.cpp

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
    include
    third/FeatherPad
)
target_link_libraries(Cutter++ PRIVATE Cutter::Cutter Rizin::Core)

install(TARGETS Cutter++ DESTINATION "${CUTTER_INSTALL_PLUGDIR}")
