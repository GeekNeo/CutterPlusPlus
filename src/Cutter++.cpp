/* Cutter++ — Interactive Modern C++ for Binary Analysis */
/* Copyright (c) 2026 Jesse Liu <neoliu2011@gmail.com> */
/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See LICENSE file in the root directory for full license text. */

#include "ICPPExec.h"
#include "Utilities.h"

#include <Cutter++.h>
#include <QString>
#include <core/Cutter.h>
#include <cstdarg>

namespace cpp {

void print(const char *format, ...) {
  va_list args;
  va_start(args, format);
  auto formatted = QString::vasprintf(format, args);
  va_end(args);
  Core()->message(QString("[Cutter++] ") + formatted);
}

int exec(const char *snippet) {
  auto code = QString(R"(#include <cutter/core/Cutter.h>
#include <Cutter++.h>
int main() {%1; return 0;})")
                  .arg(snippet);
  auto tmp = cpp::getTempSourcePath();
  if (!cpp::saveFileString(tmp, code))
    return -1;
  return ICPPExec::inst()->runSync(tmp);
}

int exec_file(const char *path) { return ICPPExec::inst()->runSync(path); }

void cpu_goto(uint64_t offset) { Core()->seek(offset); }

} // namespace cpp
