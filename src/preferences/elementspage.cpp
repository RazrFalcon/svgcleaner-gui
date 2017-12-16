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

#include "elementspage.h"
#include "ui_elementspage.h"

using namespace CleanerKey;

ElementsPage::ElementsPage(QWidget *parent)
    : BasePreferencesPage(parent)
    , ui(new Ui::ElementsPage)
{
    ui->setupUi(this);

    ui->layRmElems->setContentsMargins(leftMargin(), 0, 0, 0);
    ui->layRmDuplElems->setContentsMargins(leftMargin(), 0, 0, 0);

    // TODO: maybe rename the same as settings options
    addOptWidgets({
        { ui->chBoxRmComments, Elements::RemoveComments },
        { ui->chBoxRmDeclaration, Elements::RemoveDeclaration },
        { ui->chBoxRmNonSvg, Elements::RemoveNonSvgElements },
        { ui->chBoxRmUnusedDefs, Elements::RemoveUnusedDefs },
        { ui->chBoxConvertShapes, Elements::ConvertBasicShapes },
        { ui->chBoxRmTitle, Elements::RemoveTitle },
        { ui->chBoxRmDesc, Elements::RemoveDesc },
        { ui->chBoxRmMetadata, Elements::RemoveMetadata },
        { ui->chBoxRmDuplLinearGrad, Elements::RemoveDuplLinearGradient },
        { ui->chBoxRmDuplRadialGrad, Elements::RemoveDuplRadialGradient },
        { ui->chBoxRemoveDuplFeGaussianBlur, Elements::RemoveDuplFeGaussianBlur },
        { ui->chBoxUngroupGroups, Elements::UngroupGroups },
        { ui->chBoxUngroupDefs, Elements::UngroupDefs },
        { ui->chBoxGroupByStyle, Elements::GroupByStyle },
        { ui->chBoxMergeGradients, Elements::MergeGradients },
        { ui->chBoxRegroupGradientStops, Elements::RegroupGradientStops },
        { ui->chBoxRemoveInvalidStops, Elements::RemoveInvalidStops },
        { ui->chBoxRmInvisible, Elements::RemoveInvisibleElements },
        { ui->chBoxResolveUse, Elements::ResolveUse },
    });

    setupToolTips();
}

ElementsPage::~ElementsPage()
{
    delete ui;
}
