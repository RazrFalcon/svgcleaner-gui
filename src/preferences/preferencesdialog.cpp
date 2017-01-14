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

#include <QDialogButtonBox>
#include <QGridLayout>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QStackedWidget>

#include "src/settings.h"
#include "src/detailsdialog.h"
#include "src/preferences/widgets/iconlistview.h"

#include "mainpage.h"
#include "elementspage.h"
#include "attributespage.h"
#include "outputpage.h"
#include "pathspage.h"

#include "preferencesdialog.h"

PreferencesDialog::PreferencesDialog(QWidget *parent) : QDialog(parent)
{    
    m_listView = new IconListView();
    initList();

    MainPage *mainPage = new MainPage(this);
    ElementsPage *elementsPage = new ElementsPage(this);
    AttributesPage *attributesPage = new AttributesPage(this);
    PathsPage *pathsPage = new PathsPage(this);
    OutputPage *outputPage = new OutputPage(this);

    connect(mainPage, &MainPage::checkUpdates, this, &PreferencesDialog::checkUpdates);

    m_pages = {
        mainPage,
        elementsPage,
        attributesPage,
        pathsPage,
        outputPage,
    };

    m_stackedWidget = new QStackedWidget();

    for (auto *page : m_pages) {
        QScrollArea *area = new QScrollArea(this);
        area->setWidget(page);
        area->setWidgetResizable(true);
        area->setFrameShape(QFrame::NoFrame);

        m_stackedWidget->addWidget(area);
    }

    m_btnGenArgs = new QPushButton();
    m_btnGenArgs->setText(tr("Generate command"));
    m_btnGenArgs->setToolTip(tr("Generate command for CLI version of SVG Cleaner.\n\n"
                                "It will silently save your current preferences."));
    m_btnGenArgs->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(m_btnGenArgs, &QPushButton::clicked, this, &PreferencesDialog::onGenArgs);

    m_btnBox = new QDialogButtonBox();
    m_btnBox->addButton(QDialogButtonBox::Save);
    m_btnBox->addButton(QDialogButtonBox::RestoreDefaults);
    m_btnBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    connect(m_btnBox, &QDialogButtonBox::clicked, this, &PreferencesDialog::onBtnClicked);

    QGridLayout *lay = new QGridLayout;
    lay->addWidget(m_listView, 0, 0, 2, 1);
    lay->addWidget(m_stackedWidget, 0, 1, 1, 3);
    lay->addWidget(m_btnGenArgs, 1, 1);
    lay->addWidget(m_btnBox, 1, 2, 1, 2);
    setLayout(lay);

    connect(m_listView, &IconListView::itemSelected, this, &PreferencesDialog::onChangePage);

    AppSettings settings;
    resize(settings.value(SettingKey::PreferencesSize, QSize(640, 480)).toSize());

    m_listView->selectRow(settings.integer(SettingKey::PreferencesTab));

    setWindowTitle(tr("Preferences"));
}

PreferencesDialog::~PreferencesDialog()
{
    AppSettings settings;
    settings.setValue(SettingKey::PreferencesSize, size());
    settings.setValue(SettingKey::PreferencesTab, m_listView->selectedRow());
}

void PreferencesDialog::initList()
{
    m_listView->appendItem(tr("Main"), QIcon(":/svgcleaner.svgz"));
    m_listView->appendItem(tr("Elements"), QIcon(":/elements.svgz"));
    m_listView->appendItem(tr("Attributes"), QIcon(":/attributes.svgz"));
    m_listView->appendItem(tr("Paths"), QIcon(":/paths.svgz"));
    m_listView->appendItem(tr("Output"), QIcon(":/output.svgz"));
    m_listView->prepareSize();
}

void PreferencesDialog::onChangePage(int row)
{
    m_stackedWidget->setCurrentIndex(row);
}

void PreferencesDialog::onBtnClicked(QAbstractButton *button)
{
    if (m_btnBox->buttonRole(button) == QDialogButtonBox::AcceptRole) {
        for (BasePreferencesPage *page : m_pages) {
            page->saveConfig();
        }
        accept();
    } else if (m_btnBox->buttonRole(button) == QDialogButtonBox::ResetRole) {
        for (BasePreferencesPage *page : m_pages) {
            page->restoreDefaults();
        }
    }
}

void PreferencesDialog::onGenArgs()
{
    for (BasePreferencesPage *page : m_pages) {
        page->saveConfig();
    }

    DetailsDialog diag;
    diag.setWindowTitle(tr("Command"));
    diag.setDetails(QString("svgcleaner in.svg out.svg %1")
                    .arg(CleanerOptions::genArgs().join(' ')));
    diag.exec();
}
