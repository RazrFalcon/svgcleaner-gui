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

#pragma once

#include <QAbstractItemModel>
#include <QStyledItemDelegate>

#include "enums.h"

namespace Column {
    enum Column {
        Name,
        SizeBefore,
        SizeAfter,
        Ratio,
        Time,
        Status,
        LastColumn,
    };
}

class StatusDelegate : public QStyledItemDelegate
{
public:
    explicit StatusDelegate(QObject *parent = 0) : QStyledItemDelegate(parent) {}

private:
    void paint(QPainter *p, const QStyleOptionViewItem &opt,
               const QModelIndex &index) const;
};

struct FolderStats
{
    qint64 sizeBefore = 0;
    qint64 sizeAfter = 0;
};

struct TreeItemData {
    QString title;
    QString path;
    QString outPath;
    bool isFolder = false;

    qint64 sizeBefore = 0;
    QString sizeBeforeText;

    qint64 sizeAfter = 0;
    QString sizeAfterText;

    float ratio = 0;
    QString ratioText;

    double time = 0; // in ms
    QString timeText;

    Status status = Status::None;
    QString statusText;
};

class TreeItem
{
public:
    TreeItem(const QString &path, TreeItem *parent = 0);
    ~TreeItem();

    TreeItem *parent()                          { return m_parentItem; }
    int row() const;
    Qt::ItemFlags flags() const;

    TreeItem *child(int row)                    { return m_childItems.value(row); }
    QVector<TreeItem *> childrenList() const    { return m_childItems; }
    int childCount() const                      { return m_childItems.count(); }
    bool hasChildren() const                    { return !m_childItems.isEmpty(); }
    bool hasChild(const QString &path) const;
    bool appendChild(TreeItem *child);
    void removeChildren();

    Qt::CheckState checkState()                 { return m_checkState; }
    void setCheckState(Qt::CheckState state)    { m_checkState = state; }

    bool isEnabled() const                      { return m_isEnabled; }
    void setEnabled(bool flag)                  { m_isEnabled = flag; }

    void setSizeBefore(qint64 bytes);
    void setSizeAfter(qint64 bytes);
    void setRatio(float ratio);
    void setStatus(Status status)               { m_d.status = status; }
    void setTime(double time);
    void setStatusText(const QString &text)     { m_d.statusText = text; }
    void setOutputPath(const QString &path)     { m_d.outPath = path; }
    const TreeItemData& data() const            { return m_d; }
    bool isFolder() const                       { return m_d.isFolder; }

    void resetCleanerData();

    FolderStats calcFolderStats();
    bool hasFolderStats() const;

private:
    static QString prepareSize(qint64 bytes);

private:
    TreeItemData m_d;

    TreeItem *m_parentItem;
    QVector<TreeItem*> m_childItems;

    bool m_isEnabled = true;
    Qt::CheckState m_checkState;
};

class TreeModel : public QAbstractItemModel
{
public:
    enum class AddResult { Ok, AlreadyExists, Empty };

    explicit TreeModel(QObject *parent = 0);
    ~TreeModel();

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex index(TreeItem *item) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    void itemEditFinished(TreeItem *item);

    AddResult addFolder(const QString &path);
    TreeItem *addFile(const QString &path, TreeItem *parent = nullptr);

    TreeItem *itemByIndex(const QModelIndex &index) const;
    TreeItem *rootItem() const;

    bool isEmpty() const;
    void clear();

    void calcFoldersStats();
    int calcFileCount();

private:
    void scanFolder(const QString &path, TreeItem *parent);

private:
    TreeItem * const m_rootItem;
};
