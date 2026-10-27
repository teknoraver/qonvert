// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2007 Matteo Croce <rootkit85@yahoo.it>

#include <QApplication>
#include <QStyle>

#include "qonvert.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	// Plastique is gone, Fusion is its successor
	QApplication::setStyle("Fusion");
	QApplication::setPalette(QApplication::style()->standardPalette());
	Qonvert q;
	q.show();
	QObject::connect(&a, &QGuiApplication::lastWindowClosed, &a, &QCoreApplication::quit);
	return a.exec();
}
