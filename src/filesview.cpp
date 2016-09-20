/****************************************************************************
**
** SVG Cleaner could help you to clean up your SVG files
** from unnecessary data.
** Copyright (C) 2012-2016 Evgeniy Reizner
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License along
** with this program; if not, write to the Free Software Foundation, Inc.,
** 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
**
****************************************************************************/

#include <QDropEvent>
#include <QMimeData>
#include <QFileInfo>
#include <QMessageBox>

#include "filesview.h"

FilesView::FilesView(QWidget *parent) : QTreeView(parent)
{
    setAcceptDrops(true);
}

void FilesView::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

void FilesView::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
}

void FilesView::dropEvent(QDropEvent *event)
{

    const QMimeData *mime = event->mimeData();
    if (!mime->hasUrls()) {
        event->ignore();
        return;
    }

    for (const QUrl &url : mime->urls()) {
        if (url.isLocalFile()) {
            QString path = url.toLocalFile();
            if (QFileInfo(path).isDir()) {
                emit folderDropped(path);
            } else if (QFileInfo(path).isFile()) {
                QString suffix = QFileInfo(path).suffix().toLower();
                if (suffix == "svg" || suffix == "svgz") {
                    emit fileDropped(path);
                } else {
                    QMessageBox::warning(this, tr("Warning"),
                                         tr("You can drop only svg(z) files or folders."));
                }
            }
        }
    }
    event->acceptProposedAction();
}
