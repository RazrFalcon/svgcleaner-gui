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

#include <QProcess>
#include <QCoreApplication>

#include "process.h"

QByteArray Process::run(const QString &name, const QStringList &args,
                        int timeout, bool mergeChannels)
{
    QString path = QCoreApplication::applicationDirPath() + "/" + name;

    QProcess proc;
    if (mergeChannels) {
        proc.setProcessChannelMode(QProcess::MergedChannels);
    }

    proc.start(path, args);
    if (!proc.waitForStarted()) {
        throw tr("Process '%1' failed to start.").arg(name);
    }

    if (!proc.waitForFinished(timeout)) {
        throw tr("Process '%1' was shutdown by timeout.").arg(name);
    }

    const QByteArray output = proc.readAll();

    if (proc.exitCode() != 0) {
        throw tr("Process '%1' exit with error:\n%2").arg(name).arg(QString(output));
    }

    if (proc.exitStatus() != QProcess::NormalExit) {
        throw tr("Process '%1' was crashed:\n%2").arg(name).arg(QString(output));
    }

    return output;
}
