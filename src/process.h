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
