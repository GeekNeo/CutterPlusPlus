/* Cutter++ — Interactive Modern C++ for Binary Analysis */
/* Copyright (c) 2026 Jesse Liu <neoliu2011@gmail.com> */
/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See LICENSE file in the root directory for full license text. */

#pragma once

#include <CutterPlugin.h>
#include <QMap>

class SourceEdit;
class SnippetEdit;

class CutterPlusPlusPlugin : public QObject, CutterPlugin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "re.rizin.cutter.plugins.CutterPlugin")
  Q_INTERFACES(CutterPlugin)

public:
  void setupPlugin() override;
  void setupInterface(MainWindow *main) override;
  void terminate() override;

  QString getName() const override { return "Cutter++"; }
  QString getAuthor() const override { return "Jesse Liu"; }
  QString getDescription() const override {
    return "Interactive Modern C++ for Binary Analysis.";
  }
  QString getVersion() const override;
};

class CutterPlusPlusPluginWidget : public CutterDockWidget {
  Q_OBJECT

public:
  explicit CutterPlusPlusPluginWidget(MainWindow *main);
  virtual ~CutterPlusPlusPluginWidget() {}

private:
  void initFeatherPad(const QFont &font);
  void updateTitle(const QString &title);
  QString saveCode();

public slots:
  void onRunCode();
  void onLoad();
  void onSave();
  void onAbout();

private slots:
  void formatOnBlockChange(int) const;
  void formatOnTextChange(int, int charsRemoved, int charsAdded) const;
  void formatTextRect() const;

  // snippet edit handlers
  void onSnippetEnterPressed();
  void onSnippetArrowPressed(int key);

private:
  SourceEdit *sourceEdit;
  SnippetEdit *snippetEdit;
  QString sourcePath;
  // C++ expression history, like calculation, function calling
  QStringList snippetHistory;
  // C++ directive history, like #include
  QStringList snippetDirectives;
  // <hash, script_path>, used to skip fresh compiling to accelerate the REPL
  QMap<size_t, QString> snippetCache;
};
