/* Cutter++ — Interactive Modern C++ for Binary Analysis */
/* Copyright (c) 2026 Jesse Liu <neoliu2011@gmail.com> */
/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See LICENSE file in the root directory for full license text. */

#include "AboutDialog.h"
#include "Cutter++Plugin.h"
#include "ICPPExec.h"
#include "SourceEdit.h"
#include "Utilities.h"

#include <MainWindow.h>
#include <QDir>
#include <QFileDialog>
#include <QProcess>
#include <QTimer>
#include <common/Configuration.h>
#include <common/TempConfig.h>
#include <featherpad/highlighter/highlighter.h>

void CutterPlusPlusPlugin::setupPlugin() {}

void CutterPlusPlusPlugin::setupInterface(MainWindow *main) {
  CutterPlusPlusPluginWidget *widget = new CutterPlusPlusPluginWidget(main);
  main->addPluginDockWidget(widget);

  if (ICPPExec::inst()->init(cpp::getCurrentPluginFullPath())) {
    QStringList args;
    args << "--version";

    auto proc = new QProcess;
    proc->setProcessChannelMode(QProcess::MergedChannels);
    proc->start(ICPPExec::inst()->executable(), args);
    connect(
        proc, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this,
        [proc](int exitCode, QProcess::ExitStatus exitStatus) {
          if (exitStatus == QProcess::NormalExit && exitCode == 0)
            Core()->message(
                QString("++++++++++++++++++++++++\n%1++++++++++++++++++++++++")
                    .arg(QString::fromUtf8(proc->readAllStandardOutput())));
          proc->deleteLater();
        });
  } else {
    Core()->message(QString("Failed to load ICPP execution engine..."));
  }
}

CutterPlusPlusPluginWidget::CutterPlusPlusPluginWidget(MainWindow *main)
    : CutterDockWidget(main) {
  this->setObjectName("CutterPlusPlusPluginWidget");

  updateTitle("Anonymous");
  initFeatherPad(Config()->getFont());
}

void CutterPlusPlusPluginWidget::updateTitle(const QString &title) {
  this->setWindowTitle(QString("Cutter++ - ") + title);
}

void CutterPlusPlusPluginWidget::initFeatherPad(const QFont &font) {
  sourceEdit = new SourceEdit(font, this);
  this->setWidget(sourceEdit);

  /* visible text may change on block removal */
  connect(sourceEdit, &QPlainTextEdit::blockCountChanged, this,
          &CutterPlusPlusPluginWidget::formatOnBlockChange);
  connect(sourceEdit, &FeatherPad::TextEdit::updateRect, this,
          &CutterPlusPlusPluginWidget::formatTextRect);
  connect(sourceEdit, &FeatherPad::TextEdit::resized, this,
          &CutterPlusPlusPluginWidget::formatTextRect);
  /* this is needed when the whole visible text is pasted */
  connect(sourceEdit->document(), &QTextDocument::contentsChange, this,
          &CutterPlusPlusPluginWidget::formatOnTextChange);
}

void CutterPlusPlusPluginWidget::formatOnTextChange(int /*position*/,
                                                    int charsRemoved,
                                                    int charsAdded) const {
  if (charsRemoved > 0 || charsAdded > 0) {
    /* wait until the document's layout manager is notified about the change;
       otherwise, the end cursor might be out of range in formatTextRect() */
    QTimer::singleShot(0, this, &CutterPlusPlusPluginWidget::formatTextRect);
  }
}

void CutterPlusPlusPluginWidget::formatOnBlockChange(
    int /* newBlockCount*/) const {
  formatTextRect();
}

void CutterPlusPlusPluginWidget::formatTextRect() const {
  FeatherPad::Highlighter *highlighter =
      qobject_cast<FeatherPad::Highlighter *>(sourceEdit->getHighlighter());
  if (highlighter == nullptr)
    return;

  QPoint Point(0, 0);
  QTextCursor start = sourceEdit->cursorForPosition(Point);
  Point = QPoint(sourceEdit->width(), sourceEdit->height());
  QTextCursor end = sourceEdit->cursorForPosition(Point);

  highlighter->setLimit(start, end);
  QTextBlock block = start.block();
  while (block.isValid() && block.blockNumber() <= end.blockNumber()) {
    if (FeatherPad::TextBlockData *data =
            static_cast<FeatherPad::TextBlockData *>(block.userData())) {
      if (!data->isHighlighted()) // isn't highlighted (completely)
        highlighter->rehighlightBlock(block);
    }
    block = block.next();
  }
}

QString CutterPlusPlusPluginWidget::saveCode() {
  QString cursrc = sourcePath.isEmpty() ? cpp::getTempSourcePath() : sourcePath;
  return cpp::saveFileString(cursrc, sourceEdit->toPlainText()) ? cursrc : "";
}

void CutterPlusPlusPluginWidget::onRunCode() {
  QString srcpath = saveCode();
  if (srcpath.length())
    ICPPExec::inst()->runAsync(srcpath);
}

void CutterPlusPlusPluginWidget::onLoad() {
  QString path = QFileDialog::getOpenFileName(
      this, "Open C++ Source File", "",
      "C++ Source Files (*.cpp *.cc);;All Files (*)");
  if (path.isEmpty())
    return;

  auto code = cpp::loadFileString(path);
  if (code.length()) {
    sourcePath = path;
    sourceEdit->setPlainText(code);
    updateTitle(QFileInfo(path).fileName());
  }
}

void CutterPlusPlusPluginWidget::onSave() {
  QString path =
      QFileDialog::getSaveFileName(this, "Save C++ Source File", sourcePath,
                                   "C++ Source Files (*.cpp *.cc)");
  if (path.isEmpty())
    return;

  auto newname = sourcePath.isEmpty();
  sourcePath = path;
  saveCode();
  updateTitle(QFileInfo(path).fileName());
  Core()->message(QString("Saved %1.").arg(path));
}

void CutterPlusPlusPluginWidget::onAbout() {
  AboutDialog dlg(this);
  dlg.exec();
}
