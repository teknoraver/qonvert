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

#ifndef PROGRESS_H
#define PROGRESS_H

#include <QProcess>
#include <QDir>
#include <QList>

#include "ui_progresswidget.h"

class FileInfo
{
	public:
		FileInfo(): audio(false), video(false) {}
		QString name;
		int duration, resx, resy;
		float fps;
		bool audio, video;
};

class Job: public QProcess
{
	Q_OBJECT
	public:
		Job(const QString &t, const QString &a, FileInfo &fi, QObject *parent = nullptr, ProcessChannel o = StandardOutput): QProcess(parent), title(t), percent(0), args(a), info(fi), outputchannel(o) {};
		QString title;
		void run();

	public slots:
		virtual void readStatus() = 0;

	signals:
		void gotPercent(int);

	protected:
		int percent;
		QString args;
		FileInfo &info;
		ProcessChannel outputchannel;
};

class ProgressDialog: public QDialog, private Ui::ProgressDialog
{
	Q_OBJECT

	public:
		ProgressDialog(QWidget*);
		~ProgressDialog() override;
		QString infile, outfile;
		void run();
		bool dvdrip, norm, audio, video, cartoon;
		FileInfo info;

	private slots:
		void jobDone(int, QProcess::ExitStatus);
		void jobFailed(QProcess::ProcessError);
		void jobStarted();
		void updatePercent(int);

	private:
		int percentt, steps, step;
		QIcon converting, doneico;
		QDir workdir;
		QList<Job*> joblist;
		Job *currentjob;

		void addjob(Job*);
		void buildlist();
		void error();
};

class JobAnalyse: public Job
{
	Q_OBJECT
	public:
		JobAnalyse(const QString &t, const QString &a, FileInfo &fi, QObject *parent = nullptr, ProcessChannel o = StandardOutput):
			Job(t, a, fi, parent, o) {};
		void readStatus() override;
};

class JobAudioRip: public Job
{
	Q_OBJECT
	public:
		JobAudioRip(const QString &t, const QString &a, FileInfo &fi, QObject *parent = nullptr, ProcessChannel o = StandardOutput):
			Job(t, a, fi, parent, o) {};
		void readStatus() override;
};

class JobAudioNorm: public Job
{
	Q_OBJECT
	public:
		JobAudioNorm(const QString &t, const QString &a, FileInfo &fi, QObject *parent = nullptr, ProcessChannel o = StandardOutput):
			Job(t, a, fi, parent, o) {};
		void readStatus() override;

	private:
		bool adj;
};

class JobAudioEnc: public Job
{
	Q_OBJECT
	public:
		JobAudioEnc(const QString &t, const QString &a, FileInfo &fi, QObject *parent = nullptr, ProcessChannel o = StandardOutput):
			Job(t, a, fi, parent, o) {};
		void readStatus() override;
};

class JobVideoEnc: public Job
{
	Q_OBJECT
	public:
		JobVideoEnc(const QString &t, const QString &a, FileInfo &fi, QObject *parent = nullptr, ProcessChannel o = StandardOutput):
			Job(t, a, fi, parent, o) {};
		void readStatus() override;
};

class JobMux: public Job
{
	Q_OBJECT
	public:
		JobMux(const QString &t, const QString &a, FileInfo &fi, QObject *parent = nullptr, ProcessChannel o = StandardOutput):
			Job(t, a, fi, parent, o), steps(2 + (info.audio && info.video)) {};
		void readStatus() override;
	private:
		int steps;
};

#endif
