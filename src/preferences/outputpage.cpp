/****************************************************************************
**
** SVG Cleaner could help you to clean up your SVG files
** from unnecessary data.
** Copyright (C) 2012-2017 Evgeniy Reizner
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

#include "outputpage.h"
#include "ui_outputpage.h"

using namespace CleanerKey;

OutputPage::OutputPage(QWidget *parent)
    : BasePreferencesPage(parent)
    , ui(new Ui::OutputPage)
{
    ui->setupUi(this);

    ui->precisionLayout->setContentsMargins(leftMargin(), 0, 0, 0);

    addOptWidgets({
        { ui->chBoxRGB, Output::TrimColors },
        { ui->chBoxSimplifyTransforms, Output::SimplifyTransforms },
        { ui->spinBoxCoordsPrecision, Output::CoordinatesPrecision },
        { ui->spinBoxPropsPrecision, Output::PropertiesPrecision },
        { ui->spinBoxTsPrecision, Output::TransformsPrecision },
        { ui->spinBoxPathsPrecision, Output::PathsPrecision },
        { ui->cmbBoxListSeparator, Output::ListSeparator },
        { ui->cmbBoxIndent, Output::Indent },
    });

    ui->cmbBoxListSeparator->addItem(tr("Space"), "space");
    ui->cmbBoxListSeparator->addItem(tr("Comma"), "comma");
    ui->cmbBoxListSeparator->addItem(tr("Comma-Space"), "comma-space");

    ui->cmbBoxIndent->addItem(tr("None"), "none");
    ui->cmbBoxIndent->addItem(tr("No spaces"), "0");
    ui->cmbBoxIndent->addItem(tr("1 space"), "1");
    ui->cmbBoxIndent->addItem(tr("2 spaces"), "2");
    ui->cmbBoxIndent->addItem(tr("3 spaces"), "3");
    ui->cmbBoxIndent->addItem(tr("4 spaces"), "4");
    ui->cmbBoxIndent->addItem(tr("Tabs"), "tabs");

    setupToolTips();
}

OutputPage::~OutputPage()
{
    delete ui;
}
