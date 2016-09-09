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

#include <QDir>
#include <QElapsedTimer>

#include "utils.h"
#include "cleaner.h"
#include "process.h"

// TODO: multipass

Task::Output Task::cleanFile(const Task::Config &config)
{
    Q_ASSERT(config.inputPath.isEmpty() == false);
    Q_ASSERT(config.outputPath.isEmpty() == false);
    Q_ASSERT(config.treeItem != nullptr);

    QElapsedTimer timer;
    timer.start();

    const QString outFolder = QFileInfo(config.outputPath).absolutePath();
    if (!QFileInfo().exists(outFolder)) {
        const bool flag = QDir().mkpath(outFolder);
        if (!flag) {
            return Output::error(tr("Failed to create output folder."), config.treeItem);
        }
    }

    // take before cleaning in case of an overwrite mode
    const auto inSize = QFile(config.inputPath).size();

    // unzip svgz
    QString inputFile = config.inputPath;
    const bool isInputFileCompressed = QFileInfo(config.inputPath).suffix().toLower() == "svgz";
    if (isInputFileCompressed) {
        inputFile = config.outputPath;
        bool flag = Compressor::unzip(config.inputPath, inputFile);
        if (!flag) {
            return Output::error(tr("Failed to decompress the file."), config.treeItem);
        }
    }

    // clean file
    QStringList args;
    args.reserve(config.args.size() + 3);
    args << config.args << inputFile << config.outputPath << "--quiet=true";

    // TODO: make timeout optional
    auto res = Process::run(Cleaner::Name, args, 300000, true); // 5min
    if (!res) {
        QString msg;
        QString cmdOut = res.error().msg;
        switch (res.error().err) {
            case Process::Error::Type::None : Q_UNREACHABLE();
            case Process::Error::Type::FailedToStart:
                msg = tr("Failed to start 'svgcleaner'."); break;
            case Process::Error::Type::Timeout :
                msg = tr("'svgcleaner' was shutdown by timeout."); break;
            case Process::Error::Type::NonZeroExitCode :
            case Process::Error::Type::Crashed :
                msg = tr("'svgcleaner' was crashed:\n%1").arg(cmdOut); break;
        }
        return Output::error(msg, config.treeItem);
    }

    // process output
    const QString cleanerMsg = res.value().trimmed();

    // TODO: errors and warnings can be mixed
    if (cleanerMsg.contains("Error:")) {
        // NOTE: have to keep it in sync with CLI
        if (!cleanerMsg.contains("Cleaned file is bigger than original")) {
            return Output::error(cleanerMsg, config.treeItem);
        }
    }

    // compress file
    QString outPath = config.outputPath;

    bool shouldCompress = false;
    if (config.compressor != Compressor::None) {
        // compressor is set
        if (config.compressOnlySvgz) {
            // check that input file was SVGZ
            if (isInputFileCompressed) {
                shouldCompress = true;
            }
        } else {
            shouldCompress = true;
        }
    }

    if (shouldCompress) {
        outPath += "z";
        bool flag = Compressor(config.compressor)
                        .zip(config.compressionLevel, config.outputPath, outPath);
        if (!flag) {
            return Output::error(tr("Failed to compress the file."), config.treeItem);
        }
    }

    Output::OkData okData;
    okData.elapsed = timer.nsecsElapsed() / 1000000.0;
    okData.outSize = QFile(outPath).size();
    okData.ratio = Utils::cleanerRatio(inSize, okData.outSize);
    okData.outputPath = outPath;

    if (cleanerMsg.contains("Warning:")) {
        return Output::warning(okData, cleanerMsg, config.treeItem);
    }

    return Output::ok(okData, config.treeItem);
}
