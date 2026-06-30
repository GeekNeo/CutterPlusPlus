/* Cutter++ — Interactive Modern C++ for Binary Analysis */
/* Copyright (c) 2026 Jesse Liu <neoliu2011@gmail.com> */
/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See LICENSE file in the root directory for full license text. */

#pragma once

#include <CutterPlugin.h>

class SourceEdit;

class CutterPlusPlusPlugin : public QObject, CutterPlugin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "re.rizin.cutter.plugins.CutterPlugin")
  Q_INTERFACES(CutterPlugin)

public:
  void setupPlugin() override;
  void setupInterface(MainWindow *main) override;

  QString getName() const override { return "Cutter++"; }
  QString getAuthor() const override { return "Jesse Liu"; }
  QString getDescription() const override {
    return "Interactive Modern C++ for Binary Analysis.";
  }
  QString getVersion() const override { return "0.1.0"; }
};

class CutterPlusPlusPluginWidget : public CutterDockWidget {
  Q_OBJECT

public:
  explicit CutterPlusPlusPluginWidget(MainWindow *main);

private:
  void initFeatherPad(const QFont &font);
  void updateTitle(const QString &title);

private:
  SourceEdit *sourceEdit;

private slots:
  void formatOnBlockChange(int) const;
  void formatOnTextChange(int, int charsRemoved, int charsAdded) const;
  void formatTextRect() const;
};
