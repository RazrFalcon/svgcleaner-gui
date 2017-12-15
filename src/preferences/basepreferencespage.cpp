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
#include <QLabel>
#include <QDebug>

#include "widgets/dotwidget.h"
#include "widgets/warningcheckbox.h"

#include "basepreferencespage.h"

#define AsChBox(w) qobject_cast<QCheckBox *>(w)
#define AsWChBox(w) qobject_cast<WarningCheckBox *>(w)
#define AsCmbBox(w) qobject_cast<QComboBox *>(w)
#define AsSpinBox(w) qobject_cast<QSpinBox *>(w)

static int toIntChecked(const QVariant &value)
{
    bool ok = true;
    const int v = value.toInt(&ok);
    Q_ASSERT(ok);

    return v;
}

BasePreferencesPage::BasePreferencesPage(QWidget *parent)
    : QWidget(parent)
{}

void BasePreferencesPage::addOptWidgets(std::initializer_list<OptWidgetData> list)
{
    int idx = 0;
    for (const OptWidgetData &d : list) {
        d.w->setProperty("key", d.key);
        d.w->setProperty("index", idx);
        m_optWidgets << d.w;
        idx++;
    }

    m_dotWidgets.resize(list.size());
    int added = 0;
    for (DotWidget *w : findChildren<DotWidget*>()) {
        const int idx = toIntChecked(w->objectName().remove("dot")) - 1;
        m_dotWidgets[idx] = w;

        added++;
    }
    // Check that found dots amout is equal to widgets count.
    Q_ASSERT(added == (int)list.size());
}

int BasePreferencesPage::leftMargin() const
{
    return style()->pixelMetric(QStyle::PM_LayoutLeftMargin) * 2;
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
    for (QWidget *w : m_optWidgets) {
        const QString key = w->property("key").toString();
        if (w->inherits("QCheckBox")) {
            AsChBox(w)->setChecked(opt.flag(key));
        } else if (w->inherits("WarningCheckBox")) {
            AsWChBox(w)->setChecked(opt.flag(key));
        } else if (w->inherits("QSpinBox")) {
            AsSpinBox(w)->setValue(opt.integer(key));
        } else if (w->inherits("QComboBox")) {
            QComboBox *cmbBox = AsCmbBox(w);
            findCmbBoxItem(cmbBox, opt.string(key));
        } else {
            Q_UNREACHABLE();
        }
    }

    connectDots();
    checkDots();
}

void BasePreferencesPage::saveConfig()
{
    CleanerOptions opt;
    for (QWidget *w : m_optWidgets) {
        const QString key = w->property("key").toString();
        if (w->inherits("QCheckBox")) {
            opt.setValue(key, AsChBox(w)->isChecked());
        } else if (w->inherits("WarningCheckBox")) {
            opt.setValue(key, AsWChBox(w)->isChecked());
        } else if (w->inherits("QSpinBox")) {
            opt.setValue(key, AsSpinBox(w)->value());
        } else if (w->inherits("QComboBox")) {
            opt.setValue(key, AsCmbBox(w)->currentData());
        } else {
            Q_UNREACHABLE();
        }
    }
}

void BasePreferencesPage::setupToolTips()
{
    const Doc &doc = Doc::get();
    for (QWidget *w : m_optWidgets) {
        const QString key = w->property("key").toString();
        qobject_cast<QWidget *>(w)->setToolTip(doc.getDoc(key));
    }
}

void BasePreferencesPage::restoreDefaults()
{
    CleanerOptions opt;
    for (QWidget *w : m_optWidgets) {
        const QString key = w->property("key").toString();
        if (w->inherits("QCheckBox")) {
            AsChBox(w)->setChecked(opt.defaultFlag(key));
        } else if (w->inherits("WarningCheckBox")) {
            AsWChBox(w)->setChecked(opt.defaultFlag(key));
        } else if (w->inherits("QSpinBox")) {
            AsSpinBox(w)->setValue(opt.defaultInt(key));
        } else if (w->inherits("QComboBox")) {
            QComboBox *cmbBox = AsCmbBox(w);
            findCmbBoxItem(cmbBox, opt.defaultString(key));
        } else {
            Q_UNREACHABLE();
        }
    }
}

void BasePreferencesPage::connectDots()
{
    for (QWidget *w : m_optWidgets) {
        if (w->inherits("QCheckBox")) {
            connect(AsChBox(w), &QCheckBox::toggled,
                    this, &BasePreferencesPage::onChBoxToggled);
        } else if (w->inherits("WarningCheckBox")) {
            connect(AsWChBox(w), &WarningCheckBox::toggled,
                    this, &BasePreferencesPage::onChBoxToggled);
        } else if (w->inherits("QComboBox")) {
            connect(AsCmbBox(w), SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onCmbBoxIndexChanged(int)));
        } else if (w->inherits("QSpinBox")) {
            connect(AsSpinBox(w), SIGNAL(valueChanged(int)),
                    this, SLOT(onSpinValueChanged(int)));
        }
    }
}

void BasePreferencesPage::checkDots()
{
    for (QWidget *w : m_optWidgets) {
        if (w->inherits("QCheckBox")) {
            AsChBox(w)->toggled(AsChBox(w)->isChecked());
        } else if (w->inherits("WarningCheckBox")) {
            AsWChBox(w)->toggled(AsWChBox(w)->isChecked());
        } else if (w->inherits("QComboBox")) {
            AsCmbBox(w)->currentIndexChanged(AsCmbBox(w)->currentIndex());
        } else if (w->inherits("QSpinBox")) {
            AsSpinBox(w)->valueChanged(AsSpinBox(w)->value());
        }
    }
}

void BasePreferencesPage::checkForChanges()
{
    bool isChanged = false;

    for (DotWidget *dot : m_dotWidgets) {
        if (dot->isShowDot()) {
            isChanged = true;
            break;
        }
    }

    emit hasChanges(isChanged);
}

void BasePreferencesPage::onChBoxToggled(bool flag)
{
    const int idx = toIntChecked(sender()->property("index"));
    const QString key = sender()->property("key").toString();

    const bool isChanged = flag != CleanerOptions::defaultFlag(key);
    m_dotWidgets.at(idx)->setShowDot(isChanged);

    checkForChanges();
}

void BasePreferencesPage::onCmbBoxIndexChanged(int /*idx*/)
{
    const int idx = toIntChecked(sender()->property("index"));
    const QString key = sender()->property("key").toString();
    const QString data = AsCmbBox(sender())->currentData().toString();

    const bool isChanged = data != CleanerOptions::defaultString(key);
    m_dotWidgets.at(idx)->setShowDot(isChanged);

    checkForChanges();
}

void BasePreferencesPage::onSpinValueChanged(int value)
{
    const int idx = toIntChecked(sender()->property("index"));
    const QString key = sender()->property("key").toString();

    const bool isChanged = value != CleanerOptions::defaultInt(key);
    m_dotWidgets.at(idx)->setShowDot(isChanged);

    checkForChanges();
}
