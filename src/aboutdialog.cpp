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

#include <QScrollBar>
#include <QDesktopServices>

#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    ui->lblTitle->setText(qApp->applicationName() + " " + qApp->applicationVersion());
    ui->tabWidget->setCurrentIndex(0); // always show first tab
    fillAbout();
    fillManual();
    fillChangelog();
    fillAuthors();
    fillLicence();
    fillBuiltWith();
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::fillAbout()
{
    ui->lblAbout->setText(tr("This is a GUI for %1.")
                          .arg(genLink("svgcleaner", "https://github.com/RazrFalcon/svgcleaner")));
}

void AboutDialog::fillManual()
{
    QString link
        = genLink("here", "https://github.com/RazrFalcon/svgcleaner/blob/master/docs/svgcleaner.rst");
    ui->textManual->setHtml(tr("You can find a complete documentation %1.").arg(link));

    connect(ui->textManual, &QTextBrowser::anchorClicked, [](const QUrl &url){
        QDesktopServices::openUrl(url);
    });
}

void AboutDialog::fillChangelog()
{
    QFile file(":/changelog.html");
    file.open(QFile::ReadOnly);
    ui->textChangelog->setHtml(file.readAll());
}

void AboutDialog::fillAuthors()
{
    ui->textAuthors->append(tr("Developers:"));
    ui->textAuthors->append("Evgeniy Reizner " + genLink("razrfalcon@gmail.com"));
    ui->textAuthors->append("");
    ui->textAuthors->append(tr("Previous developers:"));
    ui->textAuthors->append("Andrey Bayrak " + genLink("andrey.bayrak@gmail.com"));
    ui->textAuthors->append("");
    ui->textAuthors->append(tr("Special thanks:"));
    ui->textAuthors->append("Alexandre Prokoudine " + genLink("alexandre.prokoudine@gmail.com"));
    ui->textAuthors->append("");
    ui->textAuthors->append(tr("Logo design:"));
    ui->textAuthors->append("Ivan Kacenkov " + genLink("kacenkov7@gmail.com"));
}

void AboutDialog::fillLicence()
{
    QFile file(":/short_gplv2.txt");
    file.open(QFile::ReadOnly);
    ui->textLicense->setPlainText(file.readAll());
}

void AboutDialog::fillBuiltWith()
{
    QString text;
    text += "<ul>";
    text += "<li>" + genLink("svgcleaner", "https://github.com/RazrFalcon/svgcleaner") + "</li>";
    text += "<li>" + genLink("Qt", "https://www.qt.io/") + "</li>";
    text += "<li>" + genLink("7-Zip", "http://www.7-zip.org/") + "</li>";
    text += "<li>" + genLink("Zopfli", "https://github.com/google/zopfli") + "</li>";
    text += "</ul>";

    ui->textBuiltWith->append(text);

    connect(ui->textBuiltWith, &QTextBrowser::anchorClicked, [](const QUrl &url){
        QDesktopServices::openUrl(url);
    });
}

QString AboutDialog::genLink(const QString &name, const QString &link)
{
    return "<a href=\"" + link + "\">"
           "<span style=\" text-decoration: underline; color:#0057ae;\">"
           + name
           + "</span></a>";
}

QString AboutDialog::genLink(const QString &link)
{
    return genLink(link, link);
}

void AboutDialog::showEvent(QShowEvent *)
{
    ui->textAuthors->verticalScrollBar()->setValue(0);
    ui->textChangelog->verticalScrollBar()->setValue(0);
    ui->textLicense->verticalScrollBar()->setValue(0);
}
