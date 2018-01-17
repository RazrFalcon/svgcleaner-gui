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

#include <QStringList>
#include <QCoreApplication>

#include "enums.h"
#include "compressor.h"

class TreeItem;

class Task
{
    Q_DECLARE_TR_FUNCTIONS(Task)

public:
    struct Config
    {
        QString inputPath;
        QString outputPath;
        QStringList args;
        TreeItem *treeItem = 0;
        Compressor::Type compressorType;
        Compressor::Level compressionLevel = Compressor::Ultra;
        bool compressOnlySvgz = false;
    };

    class Output
    {
    public:
        struct OkData
        {
            float ratio = 0;
            qint64 outSize = 0;
            QString outputPath;
        };

        struct WarningData
        {
            QString msg;
        };

        struct ErrorData
        {
            QString msg;
        };

        Output() {}
        Output(TreeItem *treeItem) : m_treeItem(treeItem) {}

        static Output ok(const OkData &data, TreeItem *treeItem)
        {
            Output s(treeItem);
            s.m_type = Status::Ok;
            s.m_ok = data;
            return s;
        }

        static Output warning(const OkData &data, const QString &warnMsg, TreeItem *treeItem)
        {
            Output s(treeItem);
            s.m_type = Status::Warning;
            s.m_ok = data;
            s.m_msg = warnMsg;
            return s;
        }

        static Output error(const QString &errMsg, TreeItem *treeItem)
        {
            Output s(treeItem);
            s.m_type = Status::Error;
            s.m_msg = errMsg;
            return s;
        }

        Status type() const
        { return m_type; }

        const OkData& okData() const
        {
            Q_ASSERT(m_type == Status::Ok || m_type == Status::Warning);
            return m_ok;
        }

        QString warningMsg() const
        {
            Q_ASSERT(m_type == Status::Warning);
            return m_msg;
        }

        QString errorMsg() const
        {
            Q_ASSERT(m_type == Status::Error);
            return m_msg;
        }

        TreeItem* item() const
        {
            Q_ASSERT(m_type != Status::None);
            return m_treeItem;
        }

    private:
        Status m_type = Status::None;
        OkData m_ok;
        QString m_msg;
        TreeItem *m_treeItem = nullptr;
    };

    static Output cleanFile(const Config &config);

private:
    static Output _cleanFile(const Config &config);
};
