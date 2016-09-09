#include <QProcess>
#include <QCoreApplication>

#include "process.h"

Result<QByteArray, Process::Error> Process::run(const QString &name, const QStringList &args,
                                                int timeout, bool mergeChannels)
{
    QString path = QCoreApplication::applicationDirPath() + "/" + name;

    QProcess proc;
    if (mergeChannels) {
        proc.setProcessChannelMode(QProcess::MergedChannels);
    }

    proc.start(path, args);
    if (!proc.waitForStarted()) {
        return Error::Type::FailedToStart;
    }

    if (!proc.waitForFinished(timeout)) {
        return Error::Type::Timeout;
    }

    const QByteArray output = proc.readAll();

    if (proc.exitCode() != 0) {
        return Error(Error::Type::NonZeroExitCode, output);
    }

    if (proc.exitStatus() != QProcess::NormalExit) {
        return Error(Error::Type::Crashed, output);
    }

    return output;
}
