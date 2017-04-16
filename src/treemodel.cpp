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

#include <QApplication>
#include <QDir>
#include <QPixmapCache>

#include "utils.h"
#include "treemodel.h"

// This model is mostly bad. I still can't understand how a tree model should be implemented.
// Especially indexes.

void StatusDelegate::paint(QPainter *p, const QStyleOptionViewItem &opt,
                           const QModelIndex &index) const
{
    QStyleOptionViewItem opt2 = opt;
    initStyleOption(&opt2, index);
    opt2.text = QString();

    const Status status = (Status)index.data().toInt();

    QPixmap pix;

    switch (status) {
        case Status::None    : break;
        case Status::Ok      : QPixmapCache::find("check.svgz", &pix); break;
        case Status::Warning : QPixmapCache::find("warning.svgz", &pix); break;
        case Status::Error   : QPixmapCache::find("error.svgz", &pix); break;
    }

    QStyle *style = QApplication::style();
    style->drawControl(QStyle::CE_ItemViewItem, &opt, p, opt.widget);

    if (!pix.isNull()) {
        QIcon icon(pix);
        QIcon::Mode mode = index.flags() & Qt::ItemIsEnabled ? QIcon::Normal : QIcon::Disabled;
        icon.paint(p, opt2.rect.adjusted(2, 2, -2, -2), Qt::AlignCenter, mode);
    }
}

TreeItem::TreeItem(const QString &path, TreeItem *parent)
{
    QFileInfo fi(path);
    if (fi.isDir()) {
        m_d.title = QDir(path).dirName();
        m_d.isFolder = true;
    } else if (fi.isFile()) {
        m_d.title = fi.fileName();
    }

    setSizeBefore(QFile(path).size());

    m_parentItem = parent;
    m_d.path = path;
    m_checkState = Qt::Checked;
}

TreeItem::~TreeItem()
{
    qDeleteAll(m_childItems);
}

bool TreeItem::appendChild(TreeItem *item)
{
    m_childItems.append(item);
    return true;
}

void TreeItem::removeChildren()
{
    qDeleteAll(m_childItems);
    m_childItems.clear();
}

FolderStats TreeItem::calcFolderStats()
{
    Q_ASSERT(isFolder() == true);

    FolderStats stats;

    for (TreeItem *child : childrenList()) {
        if (!child->isEnabled() || child->checkState() != Qt::Checked) {
            continue;
        }

        if (child->isFolder()) {
            FolderStats childStats = child->calcFolderStats();
            stats.sizeBefore += childStats.sizeBefore;
            stats.sizeAfter += childStats.sizeAfter;
        } else {
            const TreeItemData &d = child->data();
            stats.sizeBefore += d.sizeBefore;
            if (d.status != Status::Error) {
                stats.sizeAfter += d.sizeAfter;
            } else {
                // use original size on error
                stats.sizeAfter += d.sizeBefore;
            }
        }
    }

    setSizeBefore(stats.sizeBefore);
    setSizeAfter(stats.sizeAfter);
    setRatio(Utils::cleanerRatio(stats.sizeBefore, stats.sizeAfter));

    return stats;
}

bool TreeItem::hasFolderStats() const
{
    return isFolder() && m_d.sizeAfter > 0;
}

QString TreeItem::prepareSize(qint64 bytes)
{
    const qint64 kb = 1024;
    const qint64 mb = 1024 * kb;
    if (bytes >= mb) {
        return TreeModel::tr("%1 MiB").arg(QLocale().toString(qreal(bytes) / mb, 'f', 2));
    }
    if (bytes >= kb) {
        return TreeModel::tr("%1 KiB").arg(QLocale().toString(qreal(bytes) / kb, 'f', 2));
    }
    return TreeModel::tr("%1 B").arg(QLocale().toString(bytes));
}

void TreeItem::setSizeBefore(qint64 bytes)
{
    m_d.sizeBefore = bytes;
    m_d.sizeBeforeText = prepareSize(bytes);
}

void TreeItem::setSizeAfter(qint64 bytes)
{
    m_d.sizeAfter = bytes;
    m_d.sizeAfterText = prepareSize(bytes);
}

void TreeItem::setRatio(float ratio)
{
    m_d.ratio = ratio;
    m_d.ratioText = QLocale().toString(ratio, 'f', 2) + '%';
}

void TreeItem::resetCleanerData()
{
    m_d.outPath.clear();

    m_d.sizeAfter = 0;
    m_d.sizeAfterText.clear();

    m_d.ratio = 0;
    m_d.ratioText.clear();

    m_d.status = Status::None;
    m_d.statusText.clear();
}

int TreeItem::row() const
{
    if (m_parentItem) {
        return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));
    }
    return 0;
}

Qt::ItemFlags TreeItem::flags() const
{
    Qt::ItemFlags currFlags = Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
    if (!m_isEnabled) {
        currFlags &= ~(Qt::ItemIsEnabled);
    }
    return currFlags;
}

bool TreeItem::hasChild(const QString &path) const
{
    for (TreeItem *child : childrenList()) {
        if (child->data().path == path) {
            return true;
        }

        if (child->isFolder()) {
            if (child->hasChild(path)) {
                return true;
            }
        }
    }

    return false;
}

TreeModel::TreeModel(QObject *parent)
    : QAbstractItemModel(parent),
      m_rootItem(new TreeItem("Root"))
{
    QPixmapCache::insert("check.svgz", QPixmap(":/check.svgz"));
    QPixmapCache::insert("warning.svgz", QPixmap(":/warning.svgz"));
    QPixmapCache::insert("error.svgz", QPixmap(":/error.svgz"));
}

TreeModel::~TreeModel()
{
    delete m_rootItem;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    const TreeItemData &d = item->data();

    if (role == Qt::CheckStateRole && index.column() == Column::Name) {
        return item->checkState();
    }

    if (role == Qt::ToolTipRole) {
        if (index.column() == Column::Status) {
            if (d.status == Status::Warning || d.status == Status::Error) {
                return   d.statusText + "\n\n"
                       + tr("Double-click to show this text in a message box.");
            }
        }

        if (index.column() == Column::SizeBefore) {
            return tr("Double-click to open an input file.");
        }

        if (index.column() == Column::SizeAfter
                && (d.status == Status::Ok || d.status == Status::Warning)) {
            return tr("Double-click to open an output file.");
        }
    }

    if (role == Qt::TextAlignmentRole) {
        if (   index.column() == Column::SizeBefore
            || index.column() == Column::SizeAfter
            || index.column() == Column::Ratio)
        return QVariant(Qt::AlignRight | Qt::AlignVCenter);
    }

    if (role == Qt::ForegroundRole && index.column() == Column::Ratio) {
        if (d.status == Status::Ok || d.status == Status::Warning || item->hasFolderStats()) {
            QColor c;
            if (d.ratio >= 40.0f) {
                c = QColor(0, 168, 119);
            } else if (d.ratio >= 20.0f) {
                c = QColor(255, 117, 56);
            } else {
                c = QColor(242, 0, 60);
            }

            if (!item->isEnabled()) {
                c = c.darker();
            }

            return c;
        }
    }

    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    switch (index.column()) {
        case Column::Name : return d.title;
        case Column::SizeBefore : return d.sizeBeforeText;
        case Column::Status : return (int)d.status;
        default: break;
    }

    if (d.status == Status::Error) {
        return "-";
    }

    const bool isShowFolderStats = item->isFolder() && d.sizeAfter > 0;

    if (d.status == Status::Ok || d.status == Status::Warning || isShowFolderStats) {
        switch (index.column()) {
            case Column::SizeAfter : return d.sizeAfterText;
            case Column::Ratio : return d.ratioText;
            default: break;
        }
    }

    return QVariant();
}

static void setCheckToChildren(Qt::CheckState state, TreeItem *parent)
{
    for (TreeItem *child : parent->childrenList()) {
        child->setEnabled(state == Qt::Checked);
        if (child->hasChildren() && child->checkState() == Qt::Checked) {
            setCheckToChildren(state, child);
        }
    }
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::CheckStateRole && index.column() == 0) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (value.toInt() == 0) {
            item->setCheckState(Qt::Unchecked);
            setCheckToChildren(Qt::Unchecked, item);
        } else {
            item->setCheckState(Qt::Checked);
            setCheckToChildren(Qt::Checked, item);
        }
        itemEditFinished(item);
        calcFoldersStats();
    }
    return true;
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return 0;
    }

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    return item->flags();
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
            case Column::Name :         return tr("Name");
            case Column::SizeBefore :   return tr("Size before");
            case Column::SizeAfter :    return tr("Size after");
            case Column::Ratio :        return tr("Ratio");
            case Column::Status :       return tr("Status");
        default: break;
        }
    }

    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    TreeItem *parentItem;

    if (!parent.isValid()) {
        parentItem = m_rootItem;
    } else {
        parentItem = static_cast<TreeItem*>(parent.internalPointer());
    }

    TreeItem *childItem = parentItem->child(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    } else {
        return QModelIndex();
    }
}

QModelIndex TreeModel::index(TreeItem *item) const
{
    return createIndex(item->row(), 0, item);
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parent();

    if (parentItem == m_rootItem) {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0) {
        return 0;
    }

    if (!parent.isValid()) {
        parentItem = m_rootItem;
    } else {
        parentItem = static_cast<TreeItem*>(parent.internalPointer());
    }

    return parentItem->childCount();
}

int TreeModel::columnCount(const QModelIndex &) const
{
    return Column::LastColumn;
}

TreeModel::AddResult TreeModel::addFolder(const QString &path)
{
    if (rootItem()->hasChild(path)) {
        return AddResult::FolderExists;
    }

    TreeItem *dirItem = new TreeItem(path, rootItem());
    scanFolder(path, dirItem);

    if (dirItem->hasChildren()) {
        rootItem()->appendChild(dirItem);
    } else {
        delete dirItem;
        return AddResult::Empty;
    }

    return AddResult::Ok;
}

void TreeModel::scanFolder(const QString &path, TreeItem *parent)
{
    static const QStringList filesFilter = { "*.svg", "*.svgz" };

    const auto flags = QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks;
    const auto dirInfo = QDir(path).entryInfoList(flags, QDir::Name);
    for (const QFileInfo &fi : dirInfo) {
        TreeItem *dirItem = new TreeItem(fi.absoluteFilePath(), parent);
        scanFolder(fi.absoluteFilePath(), dirItem);

        if (dirItem->hasChildren()) {
            beginInsertRows(index(parent), rowCount(), rowCount());
            parent->appendChild(dirItem);
            endInsertRows();
        } else {
            // skip empty folders
            delete dirItem;
        }
    }

    for (const QFileInfo &fi : QDir(path).entryInfoList(filesFilter, QDir::Files | QDir::NoSymLinks,
                                                        QDir::Name)) {
        addFile(fi.absoluteFilePath(), parent);
    }
}

TreeModel::AddResult TreeModel::addFile(const QString &path, TreeItem *parent)
{
    if (rootItem()->hasChild(path)) {
        return AddResult::FileExists;
    }

    if (!parent) {
        parent = rootItem();
    }

    beginInsertRows(index(rowCount(), 0, QModelIndex()), rowCount(), rowCount());

    TreeItem *item = new TreeItem(path, parent);
    parent->appendChild(item);

    endInsertRows();

    return AddResult::Ok;
}

TreeItem *TreeModel::itemByIndex(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return nullptr;
    }
    return static_cast<TreeItem*>(index.internalPointer());
}

TreeItem *TreeModel::rootItem() const
{
    return m_rootItem;
}

bool TreeModel::isEmpty() const
{
    return !rootItem()->hasChildren();
}

void TreeModel::calcFoldersStats()
{
    for (TreeItem *item : rootItem()->childrenList()) {
        if (item->isFolder()) {
            item->calcFolderStats();
        }
    }
}

static int _calcFileCount(TreeItem *item)
{
    int c = 0;
    for (TreeItem *child : item->childrenList()) {
        if (child->isFolder()) {
            c += _calcFileCount(child);
        } else {
            c++;
        }
    }
    return c;
}

int TreeModel::calcFileCount()
{
    return _calcFileCount(rootItem());
}

void TreeModel::itemEditFinished(TreeItem *item)
{
    emit dataChanged(index(item->row(), 0, QModelIndex()),
                     index(item->row(), columnCount(), QModelIndex()));
}

void TreeModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, rowCount());
    m_rootItem->removeChildren();
    endRemoveRows();
}
