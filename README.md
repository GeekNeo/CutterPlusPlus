# Cutter++ — Interactive Modern C++ for Binary Analysis
Cutter++ is a plugin for [Cutter](https://github.com/rizinorg/cutter) that brings an interactive C++ REPL directly into your reverse engineering workflow.

Instead of writing Python scripts or compiling native plugins, you can write and execute modern C++23 snippets against the currently opened binary in real time.

Powered by [ICPP](https://github.com/vpand/icpp), Cutter++ turns C++ into a scripting language for reverse engineering.

## Screenshot
### Windows
![ScreenshotWin](https://raw.githubusercontent.com/GeekNeo/CutterPlusPlus/main/doc/ScreenshotWin.png)

### macOS
![ScreenshotMac](https://raw.githubusercontent.com/GeekNeo/CutterPlusPlus/main/doc/ScreenshotMac.png)

### Linux/Ubuntu
![ScreenshotLinux](https://raw.githubusercontent.com/GeekNeo/CutterPlusPlus/main/doc/ScreenshotLinux.png)

## Install
Download the package at [Release](https://github.com/GeekNeo/CutterPlusPlus/releases) page, and execute the following symlink command according to your running OS.

### Windows
```sh
mklink C:\Users\%USERNAME%\AppData\Roaming\rizin\cutter\plugins\native\Cutter++.dll DRIVE:\path\to\cutter++-xxx\Cutter++.dll
```
### macOS
```sh
ln -s -f /path/to/cutter++-xxx/Cutter++.so ~/Library/Application\ Support/rizin/cutter/plugins/native
```
### Linux/Ubuntu
```sh
ln -s -f /path/to/cutter++-xxx/Cutter++.so ~/.local/share/rizin/cutter/plugins/native
```

## Build
To build your own version of Cutter++, make sure all of the **prerequisites** are provided:
 * **CMake**;
 * **Cutter**: official Cutter product already has included headers and cmakes, so we can use it straightforward;
 * **Qt**: you should install the Qt version which can be found in Cutter's about dialog;
```sh
git clone --recursive --depth=1 https://github.com/GeekNeo/CutterPlusPlus.git
cd CutterPlusPlus
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="/path/to/Qt_DIR;/path/to/Cutter_DIR" ..
cmake --build . -- -j8
```
The newly built naked Cutter++ needs the ICPP and Qt related stuff to work. The simplest way to run your own version is to replace the Cutter++.so/dll in the official release package, otherwise you should construct the following structure:
```sh
---Cutter++.so/dll    ; Cutter plugin shared library
---Cutter++
------icpp            ; icpp execution engine
------include         ; Cutter++ headers
------QtCore          ; QtCore headers
------QtGui           ; QtGui headers
------QtWidgets       ; QtWidgets headers
```

## Issue
If you encounter any problems when using icpp, before opening an issue, please check the [Bug Report](https://github.com/GeekNeo/CutterPlusPlus/blob/main/.github/ISSUE_TEMPLATE/bug_report.md) template, and provide as many details as you can. Only if we can reproduce the problem, we can then solve it.

## Contact
If you have any questions or thoughts, just feel free to email to me:
```
neoliu2011@gmail.com
```
