/****************************************************************************
**
** SVG Cleaner could help you to clean up your SVG files
** from unnecessary data.
** Copyright (C) 2012-2017 Evgeniy Reizner
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

#include <QGuiApplication>
#include <QScreen>
#include <QIcon>
#include <QPainter>
#include <QPixmapCache>

#include "iconutils.h"

namespace IconUtils {

QPixmap renderIcon(const QString &path, int width)
{
    QPixmap pix;
    if (QPixmapCache::find(path, &pix)) {
        return pix;
    }

    const auto ratio = qApp->screens().first()->devicePixelRatio();
    width *= ratio;

    QIcon warnIcon(path);

    QImage img(width, width, QImage::Format_ARGB32);
    img.fill(Qt::transparent);
    QPainter p(&img);
    warnIcon.paint(&p, QRect(0, 0, width, width));
    p.end();

    pix = QPixmap::fromImage(img);
    QPixmapCache::insert(path, pix);

    return pix;
}

}
