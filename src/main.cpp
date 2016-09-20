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

#include <QApplication>
#include <QMessageBox>
#include <QFile>
#include <QVersionNumber>

#include "enums.h"
#include "process.h"
#include "compressor.h"
#include "mainwindow.h"

bool findCleaner();
QVersionNumber cleanerVersion();
void exeErr(const QString &name);

int main(int argc, char *argv[])
{
    const QVersionNumber ver(0, 6, 91);

    QApplication a(argc, argv);
    a.setApplicationName("SVG Cleaner");
    a.setApplicationVersion(ver.toString());

#ifdef Q_OS_MAC
    a.setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif

    if (!findCleaner()) {
        exeErr(Cleaner::Name);
        return 1;
    }

    const auto cleanerVer = cleanerVersion();
    if (ver != cleanerVer) {
        QMessageBox::critical(0, MainWindow::tr("Error"),
                              MainWindow::tr("Version mismatch:\n"
                                             "SVG Cleaner (GUI) %1\n"
                                             "svgcleaner (CLI) %2")
                              .arg(ver.toString(), cleanerVer.toString()));
        return 1;
    }

    if (!Compressor(Compressor::SevenZip).isAvailable()) {
        exeErr(CompressorName::SevenZip);
        return 1;
    }

    MainWindow w;
    w.show();

    return a.exec();
}

bool findCleaner()
{
    return !Process::run(Cleaner::Name, { "-V" }).hasError();
}

QVersionNumber cleanerVersion()
{
    auto res = Process::run(Cleaner::Name, { "-V" });
    if (!res) {
        return QVersionNumber();
    } else {
        QString out = QString::fromUtf8(*res);
        out.remove("svgcleaner ");
        return QVersionNumber::fromString(out);
    }
}

void exeErr(const QString &name)
{
    QMessageBox::critical(0, MainWindow::tr("Error"),
                          MainWindow::tr("The '%1' executable is not found.\n\n"
                                         "It should be in the application folder.")
#ifdef Q_OS_WIN
                                            .arg(name + ".exe"));
#else
                                            .arg(name));
#endif
}
