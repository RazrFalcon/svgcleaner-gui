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

#pragma once

#include <QDialog>

class QAbstractButton;
class QStackedWidget;
class QDialogButtonBox;

class IconListView;
class BasePreferencesPage;

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(QWidget *parent = 0);
    ~PreferencesDialog();

private:
    void initList();
    void onChangePage(int row);

private slots:
    void onBtnClicked(QAbstractButton *button);
    void onGenArgs();

private:
    IconListView *m_listView;
    QStackedWidget *m_stackedWidget;
    QPushButton *m_btnGenArgs;
    QDialogButtonBox *m_btnBox;

    QVector<BasePreferencesPage*> m_pages;
};
