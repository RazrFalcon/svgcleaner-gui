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

#include <QSettings>

namespace SettingKey
{
    extern const QString LastPath;
    extern const QString OutputFolder;
    extern const QString FilePrefix;
    extern const QString FileSuffix;
    extern const QString WindowSize;
    extern const QString PreferencesSize;
    extern const QString PreferencesTab;

    extern const QString SavingMethod;
    extern const QString Jobs;
    extern const QString UseCompression;
    extern const QString Compressor;
    extern const QString CompressionLevel;
    extern const QString CompressOnlySvgz;

    extern const QString CheckUpdates;
    extern const QString LastUpdatesCheck;
}

class AppSettings : public QSettings
{
public:
    AppSettings(QObject *parent = nullptr);

    enum SavingMethod
    {
        SelectFolder = 1,
        SameFolder,
        Overwrite,
    };

    bool flag(const QString &key);
    int integer(const QString &key);
    QString string(const QString &key);

    static QVariant defaultValue(const QString &key);
    static bool defaultFlag(const QString &key);
    static int defaultInt(const QString &key);
};
