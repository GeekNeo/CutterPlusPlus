/* Cutter++ — Interactive Modern C++ for Binary Analysis */
/* Copyright (c) 2026 Jesse Liu <neoliu2011@gmail.com> */
/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See LICENSE file in the root directory for full license text. */

#pragma once

#include <QString>

namespace cpp {

QString getCurrentPluginFullPath();
QString getTempSourcePath();
bool saveFileString(const QString &path, const QString &strs);

} // namespace cpp
