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

#include <QStringList>

class ProcessException : public std::exception
{
public:
    enum Type {
        FailedToStart,
        Timeout,
        NonZeroExitCode,
        Crashed,
    };

    ProcessException(const Type &e, const QString &name, const QString &text = QString())
        : m_type(e), m_name(name), m_output(text) {}

    QString explain() const;

private:
    const Type m_type;
    const QString m_name;
    const QString m_output;
};

class Process
{
public:
    static QByteArray run(const QString &name, const QStringList &args, int timeout = 30000,
                          bool mergeChannels = false);
};
