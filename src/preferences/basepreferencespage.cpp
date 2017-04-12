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

#include <QCheckBox>
#include <QSpinBox>
#include <QStyle>
#include <QComboBox>
#include <QDebug>

#include "widgets/warningcheckbox.h"
#include "basepreferencespage.h"

BasePreferencesPage::BasePreferencesPage(QWidget *parent) : QWidget(parent)
{ }

void BasePreferencesPage::addOptWidgets(std::initializer_list<OptWidgetData> list)
{
    for (const OptWidgetData &d : list) {
        d.w->setProperty("key", d.key);
        m_optWidget << d.w;
    }
}

int BasePreferencesPage::leftMargin() const
{
#ifdef Q_OS_MAC
    // 3 is too much for mac
    const int factor = 2;
#else
    const int factor = 3;
#endif
    return style()->pixelMetric(QStyle::PM_LayoutLeftMargin) * factor;
}

static void findCmbBoxItem(QComboBox *cmbBox, const QString &value)
{
    int idx = cmbBox->findData(value);
    if (idx == -1) {
        qDebug() << "Warning: combobox doesn't support such value:" << value;
        idx = 0;
    }

    cmbBox->setCurrentIndex(idx);
}

void BasePreferencesPage::loadConfig()
{
    CleanerOptions opt;
    for (QWidget *w : m_optWidget) {
        const QString key = w->property("key").toString();
        if (w->inherits("QCheckBox")) {
            qobject_cast<QCheckBox *>(w)->setChecked(opt.flag(key));
        } else if (w->inherits("WarningCheckBox")) {
            qobject_cast<WarningCheckBox *>(w)->setChecked(opt.flag(key));
        } else if (w->inherits("QSpinBox")) {
            qobject_cast<QSpinBox *>(w)->setValue(opt.integer(key));
        } else if (w->inherits("QComboBox")) {
            QComboBox *cmbBox = qobject_cast<QComboBox *>(w);
            findCmbBoxItem(cmbBox, opt.string(key));
        } else {
            Q_UNREACHABLE();
        }
    }
}

void BasePreferencesPage::saveConfig()
{
    CleanerOptions opt;
    for (QWidget *w : m_optWidget) {
        const QString key = w->property("key").toString();
        if (w->inherits("QCheckBox")) {
            opt.setValue(key, qobject_cast<QCheckBox *>(w)->isChecked());
        } else if (w->inherits("WarningCheckBox")) {
            opt.setValue(key, qobject_cast<WarningCheckBox *>(w)->isChecked());
        } else if (w->inherits("QSpinBox")) {
            opt.setValue(key, qobject_cast<QSpinBox *>(w)->value());
        } else if (w->inherits("QComboBox")) {
            opt.setValue(key, qobject_cast<QComboBox *>(w)->currentData());
        } else {
            Q_UNREACHABLE();
        }
    }
}

void BasePreferencesPage::setupToolTips()
{
    const Doc &doc = Doc::get();
    for (QWidget *w : m_optWidget) {
        const QString key = w->property("key").toString();
        qobject_cast<QWidget *>(w)->setToolTip(doc.getDoc(key));
    }
}

void BasePreferencesPage::restoreDefaults()
{
    CleanerOptions opt;
    for (QWidget *w : m_optWidget) {
        const QString key = w->property("key").toString();
        if (w->inherits("QCheckBox")) {
            qobject_cast<QCheckBox *>(w)->setChecked(opt.defaultFlag(key));
        } else if (w->inherits("WarningCheckBox")) {
            qobject_cast<WarningCheckBox *>(w)->setChecked(opt.defaultFlag(key));
        } else if (w->inherits("QSpinBox")) {
            qobject_cast<QSpinBox *>(w)->setValue(opt.defaultInt(key));
        } else if (w->inherits("QComboBox")) {
            QComboBox *cmbBox = qobject_cast<QComboBox *>(w);
            findCmbBoxItem(cmbBox, opt.defaultValue(key).toString());
        } else {
            Q_UNREACHABLE();
        }
    }
}
