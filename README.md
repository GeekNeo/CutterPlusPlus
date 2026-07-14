# Cutter++ — Interactive Modern C++ for Binary Analysis
**Cutter++** is a plugin for [Cutter](https://github.com/rizinorg/cutter) that brings an interactive **C++** REPL directly into your **reverse engineering** workflow.

Instead of writing Python scripts or compiling native plugins, you can write and execute modern C++23 snippets against the currently opened binary in real time.

Powered by [ICPP](https://github.com/vpand/icpp), **Cutter++ turns C++ into a scripting language** for reverse engineering.

ICPP allows C++ to be executed interactively with minimal startup overhead, making it practical to use C++ as an embedded scripting language.

This enables reverse engineers to use:
 * Templates
 * Concepts
 * STL
 * Ranges
 * Existing C++ headers/libraries
without leaving Cutter.

## Vision
Python made scripting popular.

Cutter++ brings the expressiveness, performance, and ecosystem of modern C++ to interactive reverse
engineering.

The goal is not to replace Python.

The goal is to make modern C++ a first-class language for binary analysis.

## Who is this for?
 * Reverse engineers
 * Malware analysts
 * Security researchers
 * CTF players
 * Compiler developers
 * Firmware researchers
 * Binary instrumentation developers

If you already think in C++, Cutter++ lets you stay in C++ while exploring binaries.

## Why?
Reverse engineers frequently need to automate repetitive analysis tasks:
 * Rename functions
 * Traverse control-flow graphs
 * Analyze cross references
 * Search for patterns
 * Patch binaries
 * Build custom analysis tools

Today there are generally two options:
 * **Python scripting**
    * Fast iteration
    * Dynamic 
    * Limited access to existing C++ libraries
    * No templates or compile-time programming 
 * **Native C++ plugins**
    * Maximum performance
    * Full language support
    * Slow development cycle
    * Requires compilation and restarting Cutter

**Cutter++ fills the gap by combining the convenience of scripting with the power of modern C++**.

## Snippet REPL
### Directive
Every snippet expression will be preset with the following directives:
```c++
#include <cutter/core/Cutter.h>
#include <Cutter++.h>
#include <icpp.hpp>
```
You can type in your own directives, they start with (exclude the ' character)
 * '#'
 * 'typedef '
 * 'using '
 * 'namespace '
 * 'import '
 * extern "C"

 e.g., include some other header file so we can call its definitions:
```c++
C++ >>> #include "/path/to/header.h"
```
```c++
C++ >>> api_in_header()
```
### Expression
You can type in any C++ expression, like calculation, function calling, etc..

The following expression will update Cutter's Disassembly window to offset 0x8000:
```c++
C++ >>> Core()->seek(0x8000)
```
### How it works
Each snippet will be automatically wrapped as:
```c++
#include ...
int main() {
  expression;
  return 0;
}
```

## Full Script
To run some complex scripts, you can write or load the full C++ script in the main editor:

### Hello World
```c++
#include <cutter/core/Cutter.h>
#include <Cutter++.h>

int main() {
    Core()->message("Hello, Cutter.");
    cpp::print("Hello, Cutter++.");
    return 0;
}
```

### Find Call
```c++
// A sample C++ script to find all the call instructions within the current
// function

#include <Cutter++.h>
#include <cutter/core/Cutter.h>

int main() {
  auto addr = Core()->getOffset();
  // get the current function
  auto func = Core()->functionIn(addr);
  if (!func)
    return -1;
  auto fnstart = rz_analysis_function_min_addr(func);
  auto fnend = rz_analysis_function_max_addr(func);
  cpp::print("Current function 0x%x - 0x%x", fnstart, fnend);

  RzAnalysisOp op{0};
  uint8_t buf[16]{0};
  auto core = Core()->lock();
  for (addr = fnstart; addr < fnend;) {
    // read the current machine opcode buffer
    rz_io_nread_at(core->io, addr, &buf[0], sizeof(buf));
    // analyze the current instruction
    auto bytes = rz_analysis_op(core->analysis, &op, addr, &buf[0], sizeof(buf),
                                RZ_ANALYSIS_OP_MASK_BASIC);
    if (bytes <= 0)
      break;
    // print the call instruction
    if (op.type == RZ_ANALYSIS_OP_TYPE_CALL)
      Core()->message(QString("%1 %2")
                          .arg(addr, 8, 16, QChar('0'))
                          .arg(Core()->disassembleSingleInstruction(addr)));
    addr += bytes;
  }
  return 0;
}
```

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

## Safety
Cutter++ executes script C++ code inside the Cutter process.

Like native plugins, executed code has the same permissions as the running application.

**Only run trusted code**.

## Build
To build your own version of Cutter++, make sure all of the **prerequisites** are provided:
 * **CMake**: plus **Ninja** if you're building for Windows;
 * **Cutter**: the official Cutter package already has included the SDK headers and cmakes, so we can use it straightforward;
 * **Qt**: you should install the Qt version which can be found in Cutter's about dialog;
```sh
git clone --recursive --depth=1 https://github.com/GeekNeo/CutterPlusPlus.git
cd CutterPlusPlus
mkdir build
cd build

# Windows (use RelWithDebInfo if you want to debug, Debug won't work because we'll be running in Cutter's Release runtime environment):
vcvarsall x64
cmake -DCMAKE_BUILD_TYPE=Release -G Ninja -DCMAKE_C_COMPILER=clang-cl -DCMAKE_CXX_COMPILER=clang-cl -DCMAKE_PREFIX_PATH="/path/to/Qt_DIR;/path/to/Cutter_DIR" ..

# macOS and Linux:
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

## API
### Implemented
```c++
// C style print log to Cutter's Console
void print(const char *format, ...);

// execute a snippet code, it will automatically wrap in a main function and add
// basic Cutter's headers
int exec(const char *snippet);

// execute a source file
int exec_file(const char *path);

// jump to the new offset in Cutter's Disassembly window
void cpu_goto(uint64_t offset);
```

### Future Planned
```sh
currentBinary()
currentFunction()
currentAddress()
functions()
imports()
exports()
strings()
sections()
segments()
comments()
xrefs()
graph()
disassemble()
decompile()
patch()
```
```c++
auto suspicious = cpp::functions()
  | std::views::filter(is_obfuscated)
  | std::views::transform(&Function::name);
for (auto &name : suspicious)
  cpp::print("Suspicious function %s", name.data());
```

## Issue
If you encounter any problems when using Cutter++, before opening an issue, please check the [Bug Report](https://github.com/GeekNeo/CutterPlusPlus/blob/main/.github/ISSUE_TEMPLATE/bug_report.md) template, and provide as many details as you can. Only if we can reproduce the problem, we can then solve it.

## Contact
If you have any questions or thoughts, just feel free to email to me:
```
neoliu2011@gmail.com
```
Any feedback is welcome.
