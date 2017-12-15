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
#include <QThread>

#include "compressor.h"
#include "settings.h"

namespace SettingKey
{
    const QString LastPath              = "LastPath";
    const QString OutputFolder          = "OutputFolder";
    const QString FilePrefix            = "FilePrefix";
    const QString FileSuffix            = "FileSuffix";
    const QString WindowSize            = "WindowSize";
    const QString PreferencesSize       = "PreferencesSize";
    const QString PreferencesTab        = "PreferencesTab";

    const QString SavingMethod          = "SavingMethod";
    const QString Jobs                  = "Jobs";
    const QString UseCompression        = "UseCompression";
    const QString Compressor            = "Compressor";
    const QString CompressionLevel      = "CompressionLevel";
    const QString CompressOnlySvgz      = "CompressOnlySvgz";

    const QString CheckUpdates          = "CheckUpdates";
    const QString LastUpdatesCheck      = "LastUpdatesCheck";
}

AppSettings::AppSettings(QObject *parent)
    : QSettings(QSettings::NativeFormat, QSettings::UserScope, "SVGCleaner", "svgcleaner", parent)
{}

bool AppSettings::flag(const QString &key)
{
    return value(key, defaultValue(key)).toBool();
}

int AppSettings::integer(const QString &key)
{
    return value(key, defaultValue(key)).toInt();
}

QString AppSettings::string(const QString &key)
{
    return value(key, defaultValue(key)).toString();
}

QVariant AppSettings::defaultValue(const QString &key)
{
    static QHash<QString, QVariant> hash;
    if (hash.isEmpty()) {
        hash.insert(SettingKey::LastPath, QDir::homePath());
        hash.insert(SettingKey::OutputFolder, QDir::homePath());
        hash.insert(SettingKey::FileSuffix, "_cleaned");
        hash.insert(SettingKey::PreferencesTab, 0);
        hash.insert(SettingKey::SavingMethod, SavingMethod::SelectFolder);
        hash.insert(SettingKey::Jobs, QThread::idealThreadCount());
        hash.insert(SettingKey::UseCompression, true);
        hash.insert(SettingKey::Compressor, CompressorName::SevenZip);
        hash.insert(SettingKey::CompressionLevel, 4);
        hash.insert(SettingKey::CompressOnlySvgz, true);
        hash.insert(SettingKey::CheckUpdates, true);
    }

    return hash.value(key);
}

bool AppSettings::defaultFlag(const QString &key)
{
    return defaultValue(key).toBool();
}

int AppSettings::defaultInt(const QString &key)
{
    return defaultValue(key).toInt();
}
