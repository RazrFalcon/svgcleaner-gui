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

#include <QThread>

#include "src/enums.h"
#include "src/settings.h"
#include "src/process.h"
#include "src/compressor.h"

#include "mainpage.h"
#include "ui_mainpage.h"

namespace CompressorTitle {
    static const QString SevenZip = "7-Zip";
    static const QString Zopfli   = "Zopfli";
}

MainPage::MainPage(QWidget *parent) :
    BasePreferencesPage(parent),
    ui(new Ui::MainPage)
{
    ui->setupUi(this);

    ui->spinBoxJobs->setMaximum(QThread::idealThreadCount());

    ui->widgetZopfliWarning->hide();
    initZip();

#ifndef WITH_CHECK_UPDATES
    ui->chBoxCheckUpdates->hide();
    ui->btnCheckUpdates->hide();
#endif
}

MainPage::~MainPage()
{
    delete ui;
}

void MainPage::initZip()
{
    ui->cmbBoxZip->addItem(CompressorTitle::SevenZip, CompressorName::SevenZip);

    try {
        Process::run(CompressorName::Zopfli, { "-h" }, 1000);
        ui->cmbBoxZip->addItem(CompressorTitle::Zopfli, CompressorName::Zopfli);
    } catch (...) {
        // ignore
    }
}

void MainPage::loadConfig()
{
    AppSettings settings;
    ui->spinBoxJobs->setValue(settings.integer(SettingKey::Jobs));
    ui->groupBoxZip->setChecked(settings.flag(SettingKey::UseCompression));

    int compressorIdx = ui->cmbBoxZip->findData(settings.string(SettingKey::Compressor));
    ui->cmbBoxZip->setCurrentIndex(compressorIdx);

    ui->cmbBoxZipLevel->setCurrentIndex(settings.integer(SettingKey::CompressionLevel));
    ui->chBoxSvgzOnly->setChecked(settings.flag(SettingKey::CompressOnlySvgz));

    ui->chBoxCheckUpdates->setChecked(settings.flag(SettingKey::CheckUpdates));

    ui->chBoxMultipass->setChecked(CleanerOptions().flag(CleanerKey::Other::Multipass));

    switch (settings.integer(SettingKey::SavingMethod)) {
        case AppSettings::SelectFolder : ui->rBtnSave1->setChecked(true); break;
        case AppSettings::SameFolder : ui->rBtnSave2->setChecked(true); break;
        case AppSettings::Overwrite : ui->rBtnSave3->setChecked(true); break;
    default: ui->rBtnSave1->setChecked(true); break;
    }

    prepareZipLvlToolTip();
}

void MainPage::saveConfig()
{
    AppSettings settings;
    settings.setValue(SettingKey::Jobs, ui->spinBoxJobs->value());
    settings.setValue(SettingKey::UseCompression, ui->groupBoxZip->isChecked());
    settings.setValue(SettingKey::Compressor, ui->cmbBoxZip->currentData());
    settings.setValue(SettingKey::CompressionLevel, ui->cmbBoxZipLevel->currentIndex());
    settings.setValue(SettingKey::CompressOnlySvgz, ui->chBoxSvgzOnly->isChecked());
    settings.setValue(SettingKey::CheckUpdates, ui->chBoxCheckUpdates->isChecked());

    int method = AppSettings::SelectFolder;
    if (ui->rBtnSave2->isChecked()) {
        method = AppSettings::SameFolder;
    } else if (ui->rBtnSave3->isChecked()) {
        method = AppSettings::Overwrite;
    }
    settings.setValue(SettingKey::SavingMethod, method);

    CleanerOptions().setValue(CleanerKey::Other::Multipass, ui->chBoxMultipass->isChecked());
}

void MainPage::restoreDefaults()
{
    AppSettings settings;
    ui->spinBoxJobs->setValue(settings.defaultInt(SettingKey::Jobs));
    ui->groupBoxZip->setChecked(settings.defaultFlag(SettingKey::UseCompression));
    ui->rBtnSave1->setChecked(true);
    ui->cmbBoxZipLevel->setCurrentIndex(settings.defaultInt(SettingKey::CompressionLevel));
    ui->chBoxSvgzOnly->setChecked(settings.defaultFlag(SettingKey::CompressOnlySvgz));

    QString compressor = settings.defaultValue(SettingKey::Compressor).toString();
    int compressorIdx = ui->cmbBoxZip->findData(compressor);
    ui->cmbBoxZip->setCurrentIndex(compressorIdx);

    ui->chBoxMultipass->setChecked(CleanerOptions().defaultFlag(CleanerKey::Other::Multipass));
}

void MainPage::on_cmbBoxZip_currentTextChanged(const QString &text)
{
    ui->widgetZopfliWarning->setVisible(text == CompressorTitle::Zopfli);
    prepareZipLvlToolTip();
}

void MainPage::on_cmbBoxZipLevel_currentIndexChanged(int /*index*/)
{
    prepareZipLvlToolTip();
}

void MainPage::prepareZipLvlToolTip()
{
    auto idx = (Compressor::Level)ui->cmbBoxZipLevel->currentIndex();
    auto c = Compressor::fromName(ui->cmbBoxZip->currentData().toString());
    ui->cmbBoxZipLevel->setToolTip(tr("Represents: %1").arg(c.levelToString(idx)));
}

void MainPage::on_btnCheckUpdates_clicked()
{
    emit checkUpdates();
}
