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

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QVersionNumber>
#include <QCoreApplication>

#include "updater.h"

Updater::Updater(QObject *parent) :
    QObject(parent),
    m_manager(new QNetworkAccessManager(this))
{
    connect(m_manager, &QNetworkAccessManager::finished,
            this, &Updater::onRequestFinished);
}

void Updater::checkUpdates()
{
    const QUrl url("https://api.github.com/repos/RazrFalcon/svgcleaner-gui/releases/latest");
    m_manager->get(QNetworkRequest(url));
}

void Updater::onRequestFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        // do nothing on error
        return;
    }

    const QByteArray ba = reply->readAll();

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(ba, &err);
    if (err.error != QJsonParseError::NoError) {
        // do nothing on error
        return;
    }

    const QVariantMap map = doc.toVariant().toMap();

    QString verStr = map.value("tag_name").toString();
    if (verStr.startsWith('v')) {
        verStr.remove(0, 1);
    }
    const QVersionNumber newVer = QVersionNumber::fromString(verStr);
    const QVersionNumber currVer = QVersionNumber::fromString(qApp->applicationVersion());

    if (newVer > currVer) {
        emit updatesFound();
    }
}
