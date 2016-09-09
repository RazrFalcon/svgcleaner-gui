#pragma once

#include <QTreeView>

class FilesView : public QTreeView
{
    Q_OBJECT

public:
    explicit FilesView(QWidget *parent = 0);

signals:
    void folderDropped(QString);
    void fileDropped(QString);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
};
