/****************************************************************************
 *   Copyright (C) 2007 Matteo Croce                                        *
 *   rootkit85@yahoo.it                                                     *
 *                                                                          *
 *   This program is free software; you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation; either version 2 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *  You should have received a copy of the GNU General Public License along *
 *  with this program; if not, write to the Free Software Foundation, Inc., *
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.             *
 ****************************************************************************/

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
