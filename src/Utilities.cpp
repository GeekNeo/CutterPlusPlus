/* Cutter++ — Interactive Modern C++ for Binary Analysis */
/* Copyright (c) 2026 Jesse Liu <neoliu2011@gmail.com> */
/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See LICENSE file in the root directory for full license text. */

#include "Utilities.h"

#include <Cutter++.h>
#include <QDir>
#include <core/Cutter.h>

#if defined(Q_OS_WIN)
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

// A dummy anchor function used exclusively to sample a local memory address
extern "C" void __CPP_EXPORT__ cutter_plus_plus() {}

namespace cpp {

QString getCurrentPluginFullPath() {
#if defined(Q_OS_WIN)
  HMODULE hModule = NULL;
  // Get the module handle matching our anchor function's memory space
  if (GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                             GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                         (LPCWSTR)&cutter_plus_plus, &hModule)) {
    wchar_t path[MAX_PATH];
    if (GetModuleFileNameW(hModule, path, MAX_PATH) > 0) {
      return QString::fromWCharArray(path);
    }
  }
#else
  // POSIX systems (Linux & macOS) use dladdr
  Dl_info info;
  if (dladdr(reinterpret_cast<void *>(&cutter_plus_plus), &info) != 0) {
    // info.dli_fname contains the absolute or canonical path to the shared
    // library
    return QDir::cleanPath(QString::fromUtf8(info.dli_fname));
  }
#endif

  return "";
}

QString getTempSourcePath() {
  return QDir::tempPath() + QDir::separator() + "Cutter++.cc";
}

QString loadFileString(const QString &path) {
  QFile file(path);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    Core()->message(QString("Could not open file: ") + file.errorString());
    return "";
  }
  QTextStream in(&file);
  in.setEncoding(QStringConverter::Utf8);
  return in.readAll();
}

bool saveFileString(const QString &path, const QString &strs) {
  QFile file(path);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    Core()->message(QString("Could not open file for writing: ") +
                    file.errorString());
    return false;
  }
  QTextStream out(&file);
  out << strs;
  return true;
}

} // namespace cpp
