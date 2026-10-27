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

#include <QFileDialog>
#include <QMessageBox>

#include "qonvert.h"
#include "progress.h"

Qonvert::Qonvert() : QMainWindow(nullptr)
{
	setupUi(this);
	connect(actionAbout, &QAction::triggered, this, &Qonvert::about);
	connect(browseIn, &QPushButton::clicked, this, &Qonvert::browseInSlot);
	connect(browseOut, &QPushButton::clicked, this, &Qonvert::browseOutSlot);
	connect(convert, &QPushButton::clicked, this, &Qonvert::convertSlot);
	c1 = new QCompleter(inName);
	c2 = new QCompleter(outName);
	dirModel.setRootPath(QString());
	c1->setModel(&dirModel);
	c2->setModel(&dirModel);
	inName->setCompleter(c1);
	outName->setCompleter(c2);
}

Qonvert::~Qonvert()
{
}

void Qonvert::browseInSlot()
{
	QString path;
	if(fileRB->isChecked())
		path = QFileDialog::getOpenFileName(this, QString(), QString(),
			"All Videos (*.avi *.mov *.mp4 *.3gp *.wmv *.asf *.mpg *.vob *.mpg *.vob);;Video Microsoft (*.avi);;MPEG4 (*.mov *.mp4 *.3gp);;Windows Media (*.wmv *.asf);;MPEG1/2 (*.mpg *.vob);;Real Media (*.rm *.rmvb);;All (*)");
	else
#ifdef WIN32
		path = QFileDialog::getExistingDirectory(this);
#else
		path = QFileDialog::getOpenFileName(this, QString(), "/dev",
			"CD/DVD drives (hd? sd? scd? sr? cdrom? dvd?);;All (*)");
#endif
	if(path.length())
		inName->setText(path);
}

void Qonvert::browseOutSlot()
{
	QString outname = inName->text();
	outname = outname.replace(outname.lastIndexOf('.'), 4, ".3gp");
	QString path(QFileDialog::getSaveFileName(this, QString(), QString(outname),
			"MPEG4 (*.mov *.mp4 *.3gp);;All (*)"));
	if(path.length())
		outName->setText(path);
}

void Qonvert::convertSlot()
{
	ProgressDialog *progress = new ProgressDialog(this);
	setVisible(false);
	progress->setVisible(true);
	progress->infile = inName->text();
	progress->outfile = outName->text();
	progress->dvdrip = dvdRB->isChecked();
	progress->audio = audioCB->isChecked();
	progress->video = videoCB->isChecked();
	progress->cartoon = cartoon->isChecked();

	connect(progress, &QDialog::rejected, this, &QWidget::show);
	connect(progress, &QDialog::accepted, this, &QWidget::show);
	progress->run();
}

void Qonvert::about()
{
	QIcon old(windowIcon());
	setWindowIcon(QIcon(":/images/matteo.png"));
	QMessageBox::about(this, "About Qonvert", "Qonvert<br><br>Brought to you by <a href=\"http://myspace.com/teknoraver\">TeknoBoy</a><br><a href=\"http://lafiorentina.free.fr/index2.htm\">Rave is not a crime<br>respect the tekno movement<br>and support free parties</a>");
	setWindowIcon(old);
}
