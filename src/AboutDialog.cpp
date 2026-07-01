/* Cutter++ — Interactive Modern C++ for Binary Analysis */
/* Copyright (c) 2026 Jesse Liu <neoliu2011@gmail.com> */
/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See LICENSE file in the root directory for full license text. */

#include "AboutDialog.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent) {
  // Set window properties
  setWindowTitle(tr("About Cutter++"));
  setFixedSize(480, 420); // Prevents unwanted resizing issues
  setWindowFlags(
      windowFlags() &
      ~Qt::WindowContextHelpButtonHint); // Removes the '?' button on Windows

  // Main layout
  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setContentsMargins(20, 20, 20, 20);
  mainLayout->setSpacing(15);

  // Title Label
  QLabel *titleLabel = new QLabel(this);
  titleLabel->setText(
      "<h2>Cutter++</h2><b>Interactive Modern C++ for Binary Analysis</b>");
  titleLabel->setTextFormat(Qt::RichText);
  mainLayout->addWidget(titleLabel);

  // Description Body
  QLabel *descLabel = new QLabel(this);
  descLabel->setText(
      "Cutter++ is a plugin for <a "
      "href='https://github.com/rizinorg/cutter'>Cutter</a> "
      "that brings an interactive C++ REPL directly into your reverse "
      "engineering workflow.<br><br>"
      "Instead of writing Python scripts or compiling native plugins, you can "
      "write and execute "
      "modern C++23 snippets against the currently opened binary in real "
      "time.<br><br>"
      "Powered by <a href='https://github.com/vpand/icpp'>ICPP</a>, Cutter++ "
      "turns C++ into "
      "a scripting language for reverse engineering.");
  descLabel->setWordWrap(true);
  descLabel->setTextFormat(Qt::RichText);
  descLabel->setOpenExternalLinks(
      true); // Ensures clicking hyperlinks opens them in a default web browser
  mainLayout->addWidget(descLabel);

  // Separator line
  QFrame *line = new QFrame(this);
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  mainLayout->addWidget(line);

  // Bottom Button Row
  QHBoxLayout *buttonLayout = new QHBoxLayout();
  QPushButton *closeButton = new QPushButton(tr("Close"), this);
  closeButton->setDefault(true);
  connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);

  buttonLayout->addStretch(); // Pushes button to the right
  buttonLayout->addWidget(closeButton);
  mainLayout->addLayout(buttonLayout);
}
