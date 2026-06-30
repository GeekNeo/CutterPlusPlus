/* Cutter++ — Interactive Modern C++ for Binary Analysis */
/* Copyright (c) 2026 Jesse Liu <neoliu2011@gmail.com> */
/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See LICENSE file in the root directory for full license text. */

#pragma once

#include <QString>
#include <string>
#include <vector>

/*
Execute a given source file at @path, @icpp should be the main icpp executable,
@opt is a clang optimization level in range {-O0, -O1, -O2, -O3}, @incptr is an
array of C++ header search argument like -I/path/to/include.
*/
typedef int (*icpp_exec_func_t)(const char *icpp, const char *path,
                                const char *opt, const char **incptr,
                                int incnum);

class ICPPExec {
public:
  static ICPPExec *inst() {
    static ICPPExec _inst;
    return &_inst;
  }

  bool init(const QString &plugin);
  void include(const QString &dir);
  void optLevel(const QString &optval);
  int run(const QString &path);

private:
  ICPPExec() {}
  ~ICPPExec() {}

  icpp_exec_func_t icpp_exec = nullptr;
  std::string icpp;
  std::string opt = "-O2";
  std::vector<std::string> hdrIncs;
};
