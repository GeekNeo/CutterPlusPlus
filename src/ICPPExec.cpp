/* Cutter++ — Interactive Modern C++ for Binary Analysis */
/* Copyright (c) 2026 Jesse Liu <neoliu2011@gmail.com> */
/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See LICENSE file in the root directory for full license text. */

#include "ICPPExec.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QLibrary>
#include <QThread>

bool ICPPExec::init(const QString &plugin) {
  if (icpp_exec)
    return true;

  QFileInfo finfo(plugin);
  auto pkgdir = finfo.absolutePath() + QDir::separator() + "Cutter++";
  // Cutter++/icpp/bin
  auto icppdir =
      pkgdir + QDir::separator() + "icpp" + QDir::separator() + "/bin";
  // parse icpp_exec api
  QLibrary libicpp(icppdir + QDir::separator() + "icpp.19");
  if (!libicpp.load())
    return false;
  icpp_exec = (icpp_exec_func_t)libicpp.resolve("icpp_exec");
  if (!icpp_exec)
    return false;

  // compose the main icpp executable
  QString icppexe = icppdir + QDir::separator() + "icpp";
#ifdef Q_OS_WIN
  icppexe += ".exe";
#endif
  icpp = icppexe.toStdString();

  // parse Cutter's include directory
  QString exeDir = QCoreApplication::applicationDirPath();
#if defined(Q_OS_WIN)
#error Unimplemented.
#elif defined(Q_OS_MACOS)
  include(exeDir + "/../Resources/include");
  include(exeDir + "/../Resources/include/cutter");
  include(exeDir + "/../Resources/include/cutter/core");
  include(exeDir + "/../Resources/include/librz");
  include(exeDir + "/../Resources/include/librz/sdb");
  include(pkgdir + "/include");
  include(pkgdir + "/qtcore");
  include(pkgdir + "/qtgui");
  include(pkgdir + "/qtwidgets");
#elif defined(Q_OS_LINUX)
#error Unimplemented.
#else
#error Unsupported OS platform.
#endif
  return true;
}

void ICPPExec::include(const QString &dir) {
  hdrIncs.push_back((QString("-I") + dir).toStdString());
}

void ICPPExec::optLevel(const QString &optval) { opt = optval.toStdString(); }

void ICPPExec::runAsync(const QString &path) {
  if (!icpp_exec)
    return;

  auto strpath = path.toStdString();
  QThread *thread = QThread::create([this, strpath]() {
    std::vector<const char *> incs;
    for (auto &i : hdrIncs)
      incs.push_back(i.data());
    icpp_exec(icpp.data(), strpath.data(), opt.data(), &incs[0],
              static_cast<int>(incs.size()));
  });
  QThread::connect(thread, &QThread::finished, thread, &QThread::deleteLater);
  thread->start();
}

int ICPPExec::runSync(const QString &path) {
  if (!icpp_exec)
    return -1;

  auto strpath = path.toStdString();
  std::vector<const char *> incs;
  for (auto &i : hdrIncs)
    incs.push_back(i.data());
  return icpp_exec(icpp.data(), strpath.data(), opt.data(), &incs[0],
                   static_cast<int>(incs.size()));
}
