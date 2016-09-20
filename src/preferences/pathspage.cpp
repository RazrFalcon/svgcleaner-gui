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

#include "pathspage.h"
#include "ui_pathspage.h"

using namespace CleanerKey;

PathsPage::PathsPage(QWidget *parent) :
    BasePreferencesPage(parent),
    ui(new Ui::PathsPage)
{
    ui->setupUi(this);

    ui->chBoxJoinArcToFlags->setStyleSheet(
        QString("WarningCheckBox { padding-left: %1 }").arg(leftMargin()));

    ui->chBoxRmUnused->setStyleSheet(
        QString("QCheckBox { padding-left: %1 }").arg(leftMargin()));

    addOptWidgets({
        { ui->chBoxToRelative, Paths::PathsToRelative },
        { ui->chBoxRmUnused, Paths::RemoveUnusedSegments },
        { ui->chBoxTrimPaths, Paths::TrimPaths },
        { ui->chBoxRmDuplCmd, Paths::RemoveDuplCmdInPaths },
        { ui->chBoxJoinArcToFlags, Paths::JoinArcToFlags },
    });

    loadConfig();
    setupToolTips();
}

PathsPage::~PathsPage()
{
    delete ui;
}

void PathsPage::loadConfig()
{
    BasePreferencesPage::loadConfig();
    on_chBoxTrimPaths_toggled(ui->chBoxTrimPaths->isChecked());
    on_chBoxToRelative_toggled(ui->chBoxToRelative->isChecked());
}

void PathsPage::on_chBoxTrimPaths_toggled(bool checked)
{
    ui->chBoxJoinArcToFlags->setEnabled(checked);
    if (!checked)
        ui->chBoxJoinArcToFlags->setChecked(false);
}

void PathsPage::on_chBoxToRelative_toggled(bool checked)
{
    ui->chBoxRmUnused->setEnabled(checked);
    if (!checked)
        ui->chBoxRmUnused->setChecked(false);
}
