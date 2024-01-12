/* Copyright (C) 2024  Sauntor <sauntor@live.com>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "QiliSubtitleLogger.h"
#include "ui_QiliSubtitleLogger.h"

#include <QDebug>
#include <QTextCursor>


QiliSubtitleLogger::QiliSubtitleLogger(QWidget *parent) :
    QiliDialog(parent),
    ui(new Ui::QiliSubtitleLogger)
{
    ui->setupUi(this);
    ui->subtitles->clear();
    auto cursor = ui->subtitles->textCursor();
    cursor.movePosition(QTextCursor::Start);
    cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    cursor.select(QTextCursor::BlockUnderCursor);
    cursor.removeSelectedText();
}

QiliSubtitleLogger::~QiliSubtitleLogger()
{
    delete ui;
}

void QiliSubtitleLogger::subtitleReceived(const QString &message)
{
    if (mMaxLines == 1) {
        ui->subtitles->clear();
    }
    int lines = ui->subtitles->document()->lineCount() - 1;
    if (lines >= mMaxLines) {
        auto cursor = ui->subtitles->textCursor();
        cursor.movePosition(QTextCursor::End);
        cursor.movePosition(QTextCursor::Up, QTextCursor::KeepAnchor);
        cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);
        cursor.select(QTextCursor::BlockUnderCursor);
        qDebug() << "Remove LINE:" << cursor.selectedText();
        cursor.removeSelectedText();
        cursor.clearSelection();
    }
    ui->subtitles->moveCursor(QTextCursor::Start);
    ui->subtitles->insertPlainText(message);
    ui->subtitles->insertPlainText("\n");
}

void QiliSubtitleLogger::setMaxLines(int maxLines)
{
    mMaxLines = maxLines;
}
