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
    installEventFilter(this);
    viewport()->installEventFilter(this);
}

void FilesView::setReadOnly(bool flag)
{
    m_readOnly = flag;
}

void FilesView::dragEnterEvent(QDragEnterEvent *event)
{
    if (!m_readOnly) {
        event->accept();
    }
}

void FilesView::dragMoveEvent(QDragMoveEvent *event)
{
    if (!m_readOnly) {
        event->accept();
    }
}

void FilesView::dropEvent(QDropEvent *event)
{
    const QMimeData *mime = event->mimeData();
    if (!mime->hasUrls()) {
        event->ignore();
        return;
    }

    for (const QUrl &url : mime->urls()) {
        if (!url.isLocalFile()) {
            continue;
        }

        QString path = url.toLocalFile();
        QFileInfo fi = QFileInfo(path);
        if (fi.isSymLink()) {
            continue;
        }

        if (fi.isDir()) {
            emit folderDropped(path);
        } else if (fi.isFile()) {
            QString suffix = QFileInfo(path).suffix().toLower();
            if (suffix == "svg" || suffix == "svgz") {
                emit fileDropped(path);
            } else {
                QMessageBox::warning(this, tr("Warning"),
                                     tr("You can drop only svg(z) files or folders."));
            }
        }
    }

    event->acceptProposedAction();
}

bool FilesView::eventFilter(QObject *obj, QEvent *event)
{
    if (m_readOnly) {
        // ignore mouse and keyboard events in readonly mode
        if (obj == viewport() && m_readOnly) {
            switch (event->type()) {
                case QEvent::MouseButtonPress :
                case QEvent::MouseButtonRelease :
                case QEvent::MouseButtonDblClick :
                case QEvent::ToolTip :
                case QEvent::ContextMenu : {
                    return true;
                }
                default: break;
            }
        } else if (obj == this && m_readOnly) {
            switch (event->type()) {
                case QEvent::KeyPress :
                case QEvent::KeyRelease : {
                    return true;
                }
                default: break;
            }
        }
    }

    return QTreeView::eventFilter(obj, event);
}
