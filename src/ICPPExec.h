/* Cutter++ — Interactive Modern C++ for Binary Analysis */
/* Copyright (c) 2026 Jesse Liu <neoliu2011@gmail.com> */
/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See LICENSE file in the root directory for full license text. */

#pragma once

#include <QSettings>
#include <QString>
#include <QStringList>
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
/*
Register a runtime library @path to icpp core engine so that scripts can call
its symbols.
*/
typedef bool (*icpp_reglib_func_t)(const char *path);

#define ICPP ICPPExec::inst()

class MainWindow;

class ICPPExec {
public:
  static ICPPExec *inst() {
    static ICPPExec _inst;
    return &_inst;
  }

  const char *executable() { return icpp.data(); }

  bool init(const QString &plugin);
  void include(const QString &dir);
  void optLevel(const QString &optval);

  void runAsync(const QString &path);
  int runSync(const QString &path);
  bool registerLibrary(const QString &path);

  void set(const QString &key, const QString &val) {
    settings.setValue(key, val);
  }

  QString get(const QString &key) { return settings.value(key).toString(); }

  QStringList snippetConfigSaved;
  int snippetCur = 0;

  MainWindow *mainWin = nullptr;
  std::string currentFile;

private:
  ICPPExec() : settings("Cutter", "Cutter++") {}
  ~ICPPExec() {}

  QSettings settings;

  icpp_exec_func_t icpp_exec = nullptr;
  icpp_reglib_func_t icpp_reglib = nullptr;
  std::string icpp;
  std::string opt = "-O2";
  std::vector<std::string> hdrIncs;
};
