/* Cutter++ — Interactive Modern C++ for Binary Analysis */
/* Copyright (c) 2026 Jesse Liu <neoliu2011@gmail.com> */
/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See LICENSE file in the root directory for full license text. */

#pragma once

#include <cstdint>

#if defined(_WIN32) || defined(_WIN64)
#define __CPP_EXPORT__ __declspec(dllexport)
#else
#define __CPP_EXPORT__ __attribute__((visibility("default")))
#endif // end of _WIN

namespace cpp {

// Cutter++ version.
constexpr const std::uint8_t version_major = 0;
constexpr const std::uint8_t version_minor = 2;
constexpr const std::uint8_t version_patch = 0;

// print log to Cutter's Console
__CPP_EXPORT__ void print(const char *format, ...);

// execute a snippet code, it will automatically wrap in a main function and add
// basic Cutter's headers
__CPP_EXPORT__ int exec(const char *snippet);

// execute a source file
__CPP_EXPORT__ int exec_file(const char *path);

// jump to the new offset in Cutter's Disassembly window
__CPP_EXPORT__ void cpu_goto(uint64_t offset);

// get the current open file path
__CPP_EXPORT__ const char *current_file();

} // namespace cpp
