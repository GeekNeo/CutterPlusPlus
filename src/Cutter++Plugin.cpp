/* Cutter++ — Interactive Modern C++ for Binary Analysis */
/* Copyright (c) 2026 Jesse Liu <neoliu2011@gmail.com> */
/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See LICENSE file in the root directory for full license text. */

#include "AboutDialog.h"
#include "Cutter++Plugin.h"
#include "ICPPExec.h"
#include "SnippetEdit.h"
#include "SourceEdit.h"
#include "Utilities.h"

#include <Cutter++.h>
#include <MainWindow.h>
#include <QDir>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QHash>
#include <QLabel>
#include <QLineEdit>
#include <QProcess>
#include <QTimer>
#include <QVBoxLayout>
#include <common/Configuration.h>
#include <common/TempConfig.h>
#include <featherpad/highlighter/highlighter.h>

static const char *config_snippet_saved = "Cutter++.Snippets";

void CutterPlusPlusPlugin::setupPlugin() {
  // load saved snippets
  auto cfgvalue = ICPP->get(config_snippet_saved);
  if (cfgvalue.size()) {
    ICPP->snippetConfigSaved = cfgvalue.split(';');
    ICPP->snippetCur = ICPP->snippetConfigSaved.size();
  }
}

void CutterPlusPlusPlugin::terminate() {
  // save the last snippets
  constexpr int max_snippet = 100;
  auto totalsize = ICPP->snippetConfigSaved.size();
  if (totalsize) {
    auto starti = totalsize > max_snippet ? totalsize - max_snippet : 0;
    QString cfgvalue;
    for (auto i = starti; i < totalsize; i++) {
      cfgvalue += ICPP->snippetConfigSaved[i] + ";";
    }
    cfgvalue.removeLast();
    ICPP->set(config_snippet_saved, cfgvalue);
  }
}

void CutterPlusPlusPlugin::setupInterface(MainWindow *main) {
  CutterPlusPlusPluginWidget *widget = new CutterPlusPlusPluginWidget(main);
  main->addPluginDockWidget(widget);

  if (ICPP->init(cpp::getCurrentPluginFullPath())) {
    QStringList args;
    args << "--version";

    auto proc = new QProcess;
    proc->setProcessChannelMode(QProcess::MergedChannels);
    proc->start(ICPP->executable(), args);
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

QString CutterPlusPlusPlugin::getVersion() const {
  return QString("%1.%2.%3")
      .arg(cpp::version_major)
      .arg(cpp::version_minor)
      .arg(cpp::version_patch);
}

CutterPlusPlusPluginWidget::CutterPlusPlusPluginWidget(MainWindow *main)
    : CutterDockWidget(main) {
  this->setObjectName("CutterPlusPlusPluginWidget");

  updateTitle("Anonymous");
  initFeatherPad(Config()->getFont());

  // preset ICPP, Cutter, and Cutter++ include directives
  snippetDirectives.append("#include <cutter/core/Cutter.h>");
  snippetDirectives.append("#include <Cutter++.h>");
  snippetDirectives.append("#include <icpp.hpp>");
}

void CutterPlusPlusPluginWidget::updateTitle(const QString &title) {
  this->setWindowTitle(QString("Cutter++ - ") + title);
}

void CutterPlusPlusPluginWidget::initFeatherPad(const QFont &font) {
  sourceEdit = new SourceEdit(font, this);
  snippetEdit = new SnippetEdit(this);

  /*
  ----------------------------------------
  |                                      |
  |               Source Edit            |
  |                                      |
  ----------------------------------------
  |C++ >> |      Snippet Edit            |
  ----------------------------------------
  */
  auto *bottomLayout = new QHBoxLayout;
  bottomLayout->addWidget(new QLabel(QStringLiteral("C++ >>>"), this));
  bottomLayout->addWidget(snippetEdit);

  auto *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(sourceEdit);
  mainLayout->addLayout(bottomLayout);

  auto *container = new QWidget(this);
  container->setLayout(mainLayout);
  this->setWidget(container);

  // connect SnippetEdit signals
  connect(snippetEdit, &SnippetEdit::enterPressed, this,
          &CutterPlusPlusPluginWidget::onSnippetEnterPressed);
  connect(snippetEdit, &SnippetEdit::arrowKeyPressed, this,
          &CutterPlusPlusPluginWidget::onSnippetArrowPressed);

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

void CutterPlusPlusPluginWidget::onSnippetEnterPressed() {
  QString snippet = snippetEdit->text();
  if (snippet.startsWith("#") || snippet.startsWith("typedef ") ||
      snippet.startsWith("using ") || snippet.startsWith("namespace ") ||
      snippet.startsWith(R"(extern "C")") || snippet.startsWith("import ")) {
    // accumulated compiler directives, like #include, #define, etc.
    if (snippet[0] != '#')
      snippet += ";";
    snippetDirectives.append(snippet);
    ICPP->snippetConfigSaved.append(snippet);
    Core()->message(QString("C++ >>> %1").arg(snippet));
  } else {
    if (!snippet.size()) {
      // repeat the last snippet
      if (!snippetHistory.size())
        return;
      snippet = snippetHistory.last();
    }
    Core()->message(QString("C++ >>> %1").arg(snippet));

    QString dyncodes;
    // the # prefixed compiler directives
    for (auto &d : snippetDirectives)
      dyncodes += d + "\n";
    // the main entry
    dyncodes += "int main(void) {" + snippet + ";return 0;}";

    // firstly check the cache
    auto hash = qHash(dyncodes);
    auto found = snippetCache.find(hash);
    if (found == snippetCache.end()) {
      auto tmpsrc = QString("%1.%2.cc").arg(cpp::getTempSourcePath()).arg(hash);
      // secondly check the script file
      if (!QFileInfo(tmpsrc).exists()) {
        // finally generate a fresh new script
        cpp::saveFileString(tmpsrc, dyncodes);
      }
      found = snippetCache.insert(hash, tmpsrc);
      // compile and run
      ICPP->runAsync(found.value());
    } else {
      // run the io cache directly
      ICPP->runSync(found.value());
    }
    if (!snippetHistory.size() || snippetHistory.last() != snippet) {
      snippetHistory.append(snippet);
      ICPP->snippetConfigSaved.append(snippet);
    }
  }
  // reset the current snippet index
  ICPP->snippetCur = ICPP->snippetConfigSaved.size();
  snippetEdit->clear();
}

void CutterPlusPlusPluginWidget::onSnippetArrowPressed(int key) {
  auto &snippets = ICPP->snippetConfigSaved;
  auto &current = ICPP->snippetCur;
  switch (key) {
  case Qt::Key_Up:
    if (0 < current && current <= snippets.size()) {
      current--;
      snippetEdit->setText(snippets[current]);
    }
    break;
  case Qt::Key_Down:
    if (0 <= current && current < snippets.size() - 1) {
      current++;
      snippetEdit->setText(snippets[current]);
    }
    break;
  default:
    break;
  }
}

QString CutterPlusPlusPluginWidget::saveCode() {
  QString cursrc = sourcePath.isEmpty() ? cpp::getTempSourcePath() : sourcePath;
  return cpp::saveFileString(cursrc, sourceEdit->toPlainText()) ? cursrc : "";
}

void CutterPlusPlusPluginWidget::onRunCode() {
  QString srcpath = saveCode();
  if (srcpath.length())
    ICPP->runAsync(srcpath);
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
  if (sourcePath.isEmpty()) {
    QString path =
        QFileDialog::getSaveFileName(this, "Save C++ Source File", sourcePath,
                                     "C++ Source Files (*.cpp *.cc)");
    if (path.isEmpty())
      return;

    sourcePath = path;
    updateTitle(QFileInfo(path).fileName());
  }

  saveCode();
  Core()->message(QString("Saved %1.").arg(sourcePath));
}

void CutterPlusPlusPluginWidget::onAbout() {
  AboutDialog dlg(this);
  dlg.exec();
}
