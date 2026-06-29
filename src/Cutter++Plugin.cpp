/* Cutter++ — Interactive Modern C++ for Binary Analysis */
/* Copyright (c) 2026 Jesse Liu <neoliu2011@gmail.com> */
/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See LICENSE file in the root directory for full license text. */

#include "Cutter++Plugin.h"

#include <QAction>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

#include <MainWindow.h>
#include <common/Configuration.h>
#include <common/TempConfig.h>
#include <rz_core.h>

void CutterPlusPlusPlugin::setupPlugin() {}

void CutterPlusPlusPlugin::setupInterface(MainWindow *main) {
  CutterPlusPlusPluginWidget *widget = new CutterPlusPlusPluginWidget(main);
  main->addPluginDockWidget(widget);
}

CutterPlusPlusPluginWidget::CutterPlusPlusPluginWidget(MainWindow *main)
    : CutterDockWidget(main) {
  this->setObjectName("CutterPlusPlusPluginWidget");
  this->setWindowTitle("Cutter++");
  QWidget *content = new QWidget();
  this->setWidget(content);

  QVBoxLayout *layout = new QVBoxLayout(content);
  content->setLayout(layout);
  text = new QLabel(content);
  text->setFont(Config()->getFont());
  text->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  layout->addWidget(text);

  QPushButton *button = new QPushButton(content);
  button->setText("Want a fortune?");
  button->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  button->setMaximumHeight(50);
  button->setMaximumWidth(200);
  layout->addWidget(button);
  layout->setAlignment(button, Qt::AlignHCenter);

  connect(Core(), &CutterCore::seekChanged, this,
          &CutterPlusPlusPluginWidget::on_seekChanged);
  connect(button, &QPushButton::clicked, this,
          &CutterPlusPlusPluginWidget::on_buttonClicked);
}

void CutterPlusPlusPluginWidget::on_seekChanged(RVA addr) {
  Q_UNUSED(addr);
  RzCoreLocked core(Core());
  TempConfig tempConfig;
  tempConfig.set("scr.color", 0);
  QString disasm = Core()->disassembleSingleInstruction(Core()->getOffset());
  QString res =
      fromOwnedCharPtr(rz_core_clippy(core, disasm.toUtf8().constData()));
  text->setText(res);
}

void CutterPlusPlusPluginWidget::on_buttonClicked() {
  RzCoreLocked core(Core());
  auto fortune = fromOwned(rz_core_fortune_get_random(core));
  if (!fortune) {
    return;
  }
  QString res = fromOwnedCharPtr(rz_core_clippy(core, fortune.get()));
  text->setText(res);
}
