/* Cutter++ — Interactive Modern C++ for Binary Analysis */
/* Copyright (c) 2026 Jesse Liu <neoliu2011@gmail.com> */
/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See LICENSE file in the root directory for full license text. */

#include "SnippetEdit.h"

#include <QKeyEvent>

SnippetEdit::SnippetEdit(QWidget *parent) : QLineEdit(parent) {
  setPlaceholderText("Type C++ expression...");
}

void SnippetEdit::keyPressEvent(QKeyEvent *event) {
  const int k = event->key();
  if (k == Qt::Key_Up || k == Qt::Key_Down)
    emit arrowKeyPressed(k);
  else if (k == Qt::Key_Return || k == Qt::Key_Enter)
    emit enterPressed();

  QLineEdit::keyPressEvent(event);
}
