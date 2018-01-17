/****************************************************************************
**
** SVG Cleaner could help you to clean up your SVG files
** from unnecessary data.
** Copyright (C) 2012-2018 Evgeniy Reizner
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

#include <QPainter>
#include <QEvent>

#include "dotwidget.h"

static const int DotWidth = 8;

DotWidget::DotWidget(QWidget *parent)
    : QWidget(parent)
{
    installEventFilter(this);

    setToolTip(tr("Current value is different from default.\n\n"
                  "It's not a warning. Just a notice."));

    setFixedSize(DotWidth, DotWidth);
}

bool DotWidget::isShowDot() const
{
    return m_isShow;
}

void DotWidget::setShowDot(bool flag)
{
    m_isShow = flag;
    update();
}

int DotWidget::leftPadding()
{
    return DotWidth;
}

void DotWidget::paintEvent(QPaintEvent *)
{
    if (!m_isShow) {
        return;
    }

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(Qt::NoPen);
    p.setBrush(palette().color(QPalette::Highlight));
    p.drawEllipse(QPoint(DotWidth / 2, DotWidth / 2), 3, 3);
}

bool DotWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == this) {
        if (event->type() == QEvent::ToolTip) {
            if (!m_isShow) {
                return true;
            }
        }
    }

    return QWidget::event(event);
}
