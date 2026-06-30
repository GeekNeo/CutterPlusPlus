/* Cutter++ — Interactive Modern C++ for Binary Analysis */
/* Copyright (c) 2026 Jesse Liu <neoliu2011@gmail.com> */
/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See LICENSE file in the root directory for full license text. */

#include "SourceEdit.h"
#include <QMenu>
#include <featherpad/config.h>
#include <featherpad/highlighter/highlighter.h>

SourceEdit::SourceEdit(const QFont &font, QWidget *parent, int bgColorValue) {
  setProg("cpp");
  setEditorFont(font);
  showLineNumbers(true);

  // Set console output context menu
  setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this, &QWidget::customContextMenuRequested, this,
          &SourceEdit::showCustomContextMenu);

  // Run current C++ code action
  runcodeAction = new QAction(tr("Run Code"), this);
  loadAction = new QAction("Load C++ File", this);
  saveAction = new QAction("Save Source Code", this);
  aboutAction = new QAction("About Cutter++", this);
  connect(runcodeAction, &QAction::triggered, this, &SourceEdit::onRunCode);

  QPoint Point(0, 0);
  QTextCursor start = cursorForPosition(Point);
  Point = QPoint(geometry().width(), geometry().height());
  QTextCursor end = cursorForPosition(Point);
  FeatherPad::Config config;
  setDrawIndetLines(config.getShowWhiteSpace());
  setVLineDistance(config.getVLineDistance());
  FeatherPad::Highlighter *highlighter = new FeatherPad::Highlighter(
      document(), "cpp", start, end, hasDarkScheme(),
      config.getShowWhiteSpace(), config.getShowEndings(),
      config.getWhiteSpaceValue(),
      config.customSyntaxColors().isEmpty() ? hasDarkScheme()
                                                  ? config.darkSyntaxColors()
                                                  : config.lightSyntaxColors()
                                            : config.customSyntaxColors());
  setHighlighter(highlighter);
}

void SourceEdit::showCustomContextMenu(const QPoint &pt) {
  QMenu menu(this);
  menu.addAction(runcodeAction);
  menu.addSeparator();
  menu.addAction(loadAction);
  menu.addAction(saveAction);
  menu.addSeparator();
  menu.addAction(aboutAction);
  menu.exec(mapToGlobal(pt));
}

void SourceEdit::onRunCode() {}
void SourceEdit::onLoad() {}
void SourceEdit::onSave() {}
void SourceEdit::onAbout() {}
