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

#include <QMainWindow>
#include <QFutureWatcher>

#include "cleaner.h"
#include "treemodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void initToolBar();
    void initTree();
    void initWatcher();
    void loadSettings();
    void saveSettings();
    void updateOutputWidget();
    void setPauseBtnVisible(bool flag);
    void setEnableGui(bool flag);
    void recalcTable();
    void addFile(const QString &path);
    void addFolder(const QString &path);

private slots:
    void onAddFiles();
    void onStart();
    void onPause();
    void onStop();
    void onResultReadyAt(int idx);
    void onFinished();
    void onDoubleClick(const QModelIndex &index);
    void on_actionAddFiles_triggered();
    void on_actionAddFolder_triggered();
    void on_actionPreferences_triggered();
    void on_actionAbout_triggered();
    void on_btnSelectFolder_clicked();
    void on_actionClearTree_triggered();

protected:
    void closeEvent(QCloseEvent *e);

private:
    Ui::MainWindow *ui;
    TreeModel * const m_model;
    QFutureWatcher<Task::Output> *m_cleaningWatcher;
};
