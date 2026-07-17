/* Cutter++ — Interactive Modern C++ for Binary Analysis */
/* Copyright (c) 2026 Jesse Liu <neoliu2011@gmail.com> */
/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See LICENSE file in the root directory for full license text. */

#include "ICPPExec.h"
#include "Utilities.h"

#include <Cutter++.h>
#include <MainWindow.h>
#include <QString>
#include <core/Cutter.h>
#include <cstdarg>

namespace cpp {

void print(const char *format, ...) {
  // remove the first \r and the last \n
  std::string_view origfmt{format};
  if (origfmt.empty())
    return;
  auto begin = format + (origfmt.front() == '\r' ? 1 : 0);
  auto end = format + origfmt.size() - (strchr("\r\n", origfmt.back()) ? 1 : 0);
  std::string strfmt{begin, end};

  va_list args;
  va_start(args, format);
  auto formatted = QString::vasprintf(strfmt.c_str(), args);
  va_end(args);

  Core()->message(formatted);
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

const char *current_file() {
  ICPP->currentFile = ICPP->mainWin->getFilename().toStdString();

  int offset = 0;
  if (ICPP->currentFile.starts_with("file://"))
    offset = 7;
  return ICPP->currentFile.c_str() + offset;
}

} // namespace cpp
