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
        throw ProcessException(ProcessException::Type::FailedToStart, name);
    }

    if (!proc.waitForFinished(timeout)) {
        throw ProcessException(ProcessException::Type::Timeout, name);
    }

    const QByteArray output = proc.readAll();

    if (proc.exitCode() != 0) {
        throw ProcessException(ProcessException::Type::NonZeroExitCode, name, output);
    }

    if (proc.exitStatus() != QProcess::NormalExit) {
        throw ProcessException(ProcessException::Type::Crashed, name, output);
    }

    return output;
}

QString ProcessException::explain() const
{
    switch (m_type) {
        case Type::FailedToStart : {
            return qApp->translate("ProcessException", "Process '%1' failed to start.")
                        .arg(m_name);
        }
        case Type::Timeout : {
            return qApp->translate("ProcessException", "Process '%1' was shutdown by timeout.")
                        .arg(m_name);
        }
        case Type::NonZeroExitCode :
        case Type::Crashed : {
            return qApp->translate("ProcessException", "Process '%1' was crashed:\n%2")
                        .arg(m_name).arg(m_output);
        }
    }

    Q_UNREACHABLE();
}
