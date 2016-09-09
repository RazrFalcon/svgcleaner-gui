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
