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

#include <QHBoxLayout>
#include <QCheckBox>
#include <QLabel>

#include "warningcheckbox.h"

WarningCheckBox::WarningCheckBox(QWidget *parent)
    : QFrame(parent),
      m_chbox(new QCheckBox()),
      m_lbl(new QLabel())
{
    m_chbox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    m_lbl->setPixmap(QPixmap(":/warning.svgz"));
    m_lbl->setScaledContents(true);

    int l = fontMetrics().height() * 0.9;
    m_lbl->setFixedSize(l, l);

    QHBoxLayout *lay = new QHBoxLayout();
    lay->setContentsMargins(0, 0, 0, 0);
    lay->addWidget(m_chbox);
    lay->addWidget(m_lbl);
    lay->addStretch();
    setLayout(lay);
}

void WarningCheckBox::setText(const QString &text)
{
    m_chbox->setText(text);
}

bool WarningCheckBox::isChecked() const
{
    return m_chbox->isChecked();
}

void WarningCheckBox::setChecked(bool flag)
{
    m_chbox->setChecked(flag);
}
