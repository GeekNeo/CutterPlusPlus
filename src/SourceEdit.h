/* Cutter++ — Interactive Modern C++ for Binary Analysis */
/* Copyright (c) 2026 Jesse Liu <neoliu2011@gmail.com> */
/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See LICENSE file in the root directory for full license text. */

#pragma once

#include <featherpad/textedit.h>

class QAction;
class CutterPlusPlusPluginWidget;

class SourceEdit : public FeatherPad::TextEdit {
  Q_OBJECT

public:
  SourceEdit(const QFont &font, CutterPlusPlusPluginWidget *parent,
             int bgColorValue = 255);
  virtual ~SourceEdit() {}

protected:
  bool event(QEvent *event) override;

private slots:
  void showCustomContextMenu(const QPoint &pt);

private:
  QAction *runcodeAction;
  QAction *loadAction;
  QAction *saveAction;
  QAction *aboutAction;
};
