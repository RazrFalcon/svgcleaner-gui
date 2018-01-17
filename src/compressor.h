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

#pragma once

#include <QString>

namespace CompressorName
{
    extern const QString SevenZip;
    extern const QString Zopfli;
}

class Compressor
{
public:
    enum Type
    {
        None,
        SevenZip,
        Zopfli,
    };

    enum Level
    {
        Lowest,
        Low,
        Normal,
        Optimal,
        Ultra,
    };

    Compressor(Type t) : m_type(t) {}
    static Compressor fromName(const QString &aname) noexcept;

    bool isAvailable() const;
    QString levelToString(Level v) const noexcept;
    QString name() const noexcept;
    Type type() const noexcept
    { return m_type; }

    void zip(Level lvl, const QString &inFile, const QString &outFile) const;
    static void unzip(const QString &inFile, const QString &outFile);

private:
    Type m_type = None;
};

