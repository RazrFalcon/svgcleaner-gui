#pragma once

#include <QListView>

class IconListModel;

class IconListView : public QListView
{
    Q_OBJECT

public:
    explicit IconListView(QWidget *parent = 0);

    void appendItem(const QString &title, const QIcon &icon);

    int selectedRow() const;
    void selectRow(int row) const;

    void prepareSize();

signals:
    void itemSelected(int);

public slots:

private:
    IconListModel * const m_model;
};
