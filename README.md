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
### Windows
```sh
mklink C:\Users\%USERNAME%\AppData\Roaming\rizin\cutter\plugins\native\Cutter++.dll DRIVE:\path\to\Cutter++.dll
```
### Linux/Ubuntu
```sh
ln -s -f /path/to/cutter++-xxx/Cutter++.so ~/.local/share/rizin/cutter/plugins/native
```
