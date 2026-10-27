// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2007 Matteo Croce <rootkit85@yahoo.it>

#ifndef QONVERT_H
#define QONVERT_H

#include <QMainWindow>
#include <QCompleter>
#include <QFileSystemModel>
#include <QThread>
#include <QProcess>

#include "ui_qonvertwidget.h"

class Qonvert: public QMainWindow, private Ui::MainWindow
{
	Q_OBJECT

public:
	Qonvert();
	~Qonvert() override;

private slots:
	void about();
	void browseInSlot();
	void browseOutSlot();
	void convertSlot();

private:
	QCompleter *c1, *c2;
	QFileSystemModel dirModel;
};

#endif
