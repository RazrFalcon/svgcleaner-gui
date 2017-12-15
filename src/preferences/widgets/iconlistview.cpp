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

#include <QStyledItemDelegate>
#include <QApplication>
#include <QAbstractListModel>
#include <QPainter>
#include <QDebug>

#include "iconlistview.h"

class IconDelegate : public QStyledItemDelegate
{
public:
    explicit IconDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    static int widthHint()
    { return iconSize + iconSpacing * 2; }

    static int heightHint(const QFontMetrics &fm)
    { return iconSize + fm.height() + iconSpacing * 3; }

private:
    void paint(QPainter *p, const QStyleOptionViewItem &opt,
               const QModelIndex &index) const
    {
        const QIcon icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
        const bool isShowDot = index.data(Qt::UserRole).toBool();

        QStyle *style = QApplication::style();
        style->drawControl(QStyle::CE_ItemViewItem, &opt, p, opt.widget);

        Q_ASSERT(!icon.isNull());

        if (!icon.isNull()) {
            QRect r = opt.rect;
            r.translate((r.width() - iconSize) / 2, iconSpacing);
            r.setSize(QSize(iconSize, iconSize));

            icon.paint(p, r);
        }

        if (isShowDot) {
            p->setRenderHint(QPainter::Antialiasing);
            p->setPen(Qt::NoPen);
#ifdef Q_OS_WIN
            const auto role = QPalette::Highlight;
#else
            const auto role = opt.state & QStyle::State_Selected ? QPalette::Text
                                                                 : QPalette::Highlight;
#endif
            p->setBrush(opt.palette.color(role));
            // TODO: fix magic numbers
            p->drawEllipse(opt.rect.topLeft() + QPoint(13 + iconSpacing, 6), 4, 4);
        }

#ifdef Q_OS_WIN
        const auto role = QPalette::Text;
#else
        const auto role = opt.state & QStyle::State_Selected ? QPalette::HighlightedText
                                                             : QPalette::Text;
#endif
        p->setPen(opt.palette.color(role));
        p->setBrush(Qt::NoBrush);

        p->drawText(opt.rect.adjusted(0, 0, 0, -iconSpacing), Qt::AlignHCenter | Qt::AlignBottom,
                    index.data().toString());
    }

    QSize sizeHint(const QStyleOptionViewItem &opt, const QModelIndex &/*index*/) const
    {
        Q_ASSERT(opt.widget != nullptr);
        return QSize(widthHint(), heightHint(opt.widget->fontMetrics()));
    }

private:
    static constexpr int iconSize = 64;
    static constexpr int iconSpacing = 4;
};

class IconListModel : public QAbstractListModel
{
public:
    explicit IconListModel(QObject *parent = nullptr) : QAbstractListModel(parent) {}

    int rowCount(const QModelIndex &parent = QModelIndex()) const
    { Q_UNUSED(parent); return m_data.count(); }

    QVariant data(const QModelIndex &index, int role) const
    {
        if (role == Qt::DecorationRole) {
            return m_data.at(index.row()).icon;
        }

        if (role == Qt::DisplayRole) {
            return m_data.at(index.row()).title;
        }

        if (role == Qt::UserRole) {
            return m_data.at(index.row()).showDot;
        }

        return QVariant();
    }

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole)
    {
        Q_ASSERT(role == Qt::UserRole);

        m_data[index.row()].showDot = value.toBool();

        dataChanged(index, index);

        return true;
    }

    void appendItem(const QString &title, const QIcon &icon)
    {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        m_data << Data(title, icon);
        endInsertRows();
    }

private:
    struct Data
    {
        QString title;
        QIcon icon;
        bool showDot = false;

        Data() {}
        Data(const QString &atitle, const QIcon &aicon) : title(atitle), icon(aicon) {}
    };

private:
    QVector<Data> m_data;
};

IconListView::IconListView(QWidget *parent)
    : QListView(parent),
      m_model(new IconListModel(this))
{
    setModel(m_model);
    setItemDelegate(new IconDelegate(this));

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(selectionModel(), &QItemSelectionModel::currentRowChanged,
        [this](const QModelIndex &idx) {
            emit itemSelected(idx.row());
        }
    );
}

void IconListView::appendItem(const QString &title, const QIcon &icon)
{
    m_model->appendItem(title, icon);
}

int IconListView::selectedRow() const
{
    return selectionModel()->selectedRows().first().row();
}

void IconListView::selectRow(int row) const
{
    selectionModel()->setCurrentIndex(m_model->index(row), QItemSelectionModel::SelectCurrent);
}

void IconListView::prepareSize()
{
    int max = IconDelegate::widthHint();
    for (int row = 0; row < m_model->rowCount(); row++) {
        const QString title = m_model->data(m_model->index(row, 0), Qt::DisplayRole).toString();
        max = qMax(fontMetrics().width(title), max);
    }
    setFixedWidth(max * 1.2);

    setMinimumHeight(m_model->rowCount() * IconDelegate::heightHint(fontMetrics())
                     + fontMetrics().height() * 2);
}

void IconListView::setShowDot(int idx, bool flag)
{
    Q_ASSERT(idx < m_model->rowCount());

    m_model->setData(m_model->index(idx, 0), flag, Qt::UserRole);

    update();
}
