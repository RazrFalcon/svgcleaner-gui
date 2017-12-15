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

PathsPage::PathsPage(QWidget *parent)
    : BasePreferencesPage(parent)
    , ui(new Ui::PathsPage)
{
    ui->setupUi(this);

    ui->laySegments->setContentsMargins(leftMargin(), 0, 0, 0);
    ui->layArcTo->setContentsMargins(leftMargin(), 0, 0, 0);

    addOptWidgets({
        { ui->chBoxToRelative, Paths::PathsToRelative },
        { ui->chBoxRmUnused, Paths::RemoveUnusedSegments },
        { ui->chBoxConvertSegments, Paths::ConvertSegments },
        { ui->chBoxApplyTransformToPaths, Paths::ApplyTransformToPaths },
        { ui->chBoxTrimPaths, Paths::TrimPaths },
        { ui->chBoxJoinArcToFlags, Paths::JoinArcToFlags },
        { ui->chBoxRmDuplCmd, Paths::RemoveDuplCmdInPaths },
        { ui->chBoxUseImplicitCommands, Paths::UseImplicitCommands },
    });

    setupToolTips();
}

PathsPage::~PathsPage()
{
    delete ui;
}

void PathsPage::loadConfig()
{
    BasePreferencesPage::loadConfig();

    updateChildren();

    // Connect only after config is loaded because this slots should be invoked only by a user.
    connect(ui->chBoxToRelative, &QCheckBox::toggled, this, &PathsPage::onChBoxToRelativeToggled);
    connect(ui->chBoxTrimPaths, &QCheckBox::toggled, this, &PathsPage::onChBoxTrimPathsToggled);
}

void PathsPage::updateChildren()
{
    {
        const bool flag = ui->chBoxToRelative->isChecked();
        ui->chBoxRmUnused->setEnabled(flag);
        ui->chBoxConvertSegments->setEnabled(flag);
        ui->chBoxApplyTransformToPaths->setEnabled(flag);
    }

    {
        const bool flag = ui->chBoxTrimPaths->isChecked();
        ui->chBoxJoinArcToFlags->setEnabled(flag);
    }
}

static void prepareChBox(QCheckBox *chBox, bool checked)
{
    if (!checked) {
        chBox->setChecked(false);
    } else {
        chBox->setChecked(CleanerOptions::defaultFlag(chBox->property("key").toString()));
    }
}

void PathsPage::onChBoxToRelativeToggled(bool checked)
{
    prepareChBox(ui->chBoxRmUnused, checked);
    prepareChBox(ui->chBoxConvertSegments, checked);
    prepareChBox(ui->chBoxApplyTransformToPaths, checked);

    updateChildren();
}

void PathsPage::onChBoxTrimPathsToggled(bool checked)
{
    // Set check manually because chBoxJoinArcToFlags is not a QCheckBox.
    ui->chBoxJoinArcToFlags->setEnabled(checked);
    if (!checked) {
        ui->chBoxJoinArcToFlags->setChecked(CleanerOptions::defaultFlag(Paths::JoinArcToFlags));
    }

    updateChildren();
}
