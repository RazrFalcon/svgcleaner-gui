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

#include "result.h"

class Process
{
public:
    struct Error {
        enum class Type {
            None,
            FailedToStart,
            Timeout,
            NonZeroExitCode,
            Crashed,
        };

        Error() : err(Type::None) {}
        Error(const Type &e, const QString &amsg) : err(e), msg(amsg) {}

        Type err;
        QString msg;
    };

    static Result<QByteArray, Error> run(const QString &name, const QStringList &args,
                                         int timeout = 30000, bool mergeChannels = false);
};
