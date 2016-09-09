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

#include <QFile>
#include <QDataStream>

#include "doc.h"

Doc::Doc()
{
    QFile file(":/doc.dat");
    file.open(QFile::ReadOnly); // it can't fail

    QByteArray ba = qUncompress(file.readAll());

    QDataStream stream(&ba, QFile::ReadOnly);
    while (!stream.atEnd()) {
        QString key;
        QByteArray value;

        stream >> key;
        stream >> value;

        m_values.insert(key, value);
    }
}

QString Doc::getDoc(const QString &key) const
{
    Q_ASSERT_X(m_values.contains(key) == true, "Doc", qPrintable("unknown key: " + key));
    return m_values.value(key);
}
