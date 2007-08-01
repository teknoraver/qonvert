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

#include <QApplication>
#include <QMainWindow>

#include <QPlastiqueStyle>
#include <QDesktopWidget>
#include "qonvert.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QApplication::setStyle(new QPlastiqueStyle());
	QApplication::setPalette(QApplication::style()->standardPalette());
	Qonvert q;
	q.move((QApplication::desktop()->screenGeometry(&q).width() - q.width()) / 2, (QApplication::desktop()->screenGeometry(&q).height() - q.height()) / 2);
	q.setVisible(true);
	QObject::connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
	return a.exec();
}
