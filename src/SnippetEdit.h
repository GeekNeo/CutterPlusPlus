/* Cutter++ — Interactive Modern C++ for Binary Analysis */
/* Copyright (c) 2026 Jesse Liu <neoliu2011@gmail.com> */
/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See LICENSE file in the root directory for full license text. */

#pragma once

#include <QLineEdit>

class SnippetEdit : public QLineEdit {
  Q_OBJECT

public:
  explicit SnippetEdit(QWidget *parent = nullptr);
  virtual ~SnippetEdit() {}

signals:
  void arrowKeyPressed(int key); // emits Qt::Key_*
  void enterPressed();

protected:
  void keyPressEvent(QKeyEvent *event) override;
};
