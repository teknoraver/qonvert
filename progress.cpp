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

#include <QMessageBox>
#include <QListWidgetItem>
#include <QRegularExpression>
#include <QDir>

#ifdef WIN32
#include <windows.h>
#define getPid() GetCurrentProcessId()
#else
#include <sys/types.h>
#include <unistd.h>
#define getPid() getpid()
#endif

#include "progress.h"

ProgressDialog::ProgressDialog(QWidget *parent) : QDialog(parent, Qt::Window), dvdrip(false), norm(true), audio(false), video(false), cartoon(false), percentt(0), step(0), converting(":/images/qonvert.png"), doneico(":/images/ok.png")
{
	setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
}

ProgressDialog::~ProgressDialog()
{
	QStringList list = workdir.entryList(QDir::Files);
	foreach(QString str, list)
		workdir.remove(str);

	workdir.rmdir(workdir.path());

	QDir::setCurrent(QDir::homePath());
}

void ProgressDialog::run()
{
	workdir = QDir::temp();
	workdir.mkdir("qonvert." + QString::number(getPid()));
	workdir.cd("qonvert." + QString::number(getPid()));

//	qDebug("tempdir: %s", qPrintable(workdir.path()));
	QDir::setCurrent(workdir.path());

	info.name = infile;
	buildlist();
	steps = joblist.count();
	currentjob = joblist.takeFirst();
	currentjob->run();
	currentp->setValue(0);
	totalp->setValue(0);
}

void ProgressDialog::updatePercent(int p)
{
//	qDebug("ProgressDialog::updatePercent(%d)", p);
	currentp->setValue(p);
	int newtotalp = percentt + p / steps;
	if(percentt != newtotalp)
		totalp->setValue(newtotalp);
}

void ProgressDialog::jobFailed(QProcess::ProcessError)
{
//	qDebug("ProgressDialog::jobFailed(%d)", e);
	error();
}

void ProgressDialog::error()
{
//	qDebug("ProgressDialog::error()");
	QMessageBox::critical(this, "Error", "Encoding failed");
//	qDebug("ProgressDialog::close()");
	close();
}

void ProgressDialog::addjob(Job *job)
{
	connect(job, &Job::gotPercent, this, &ProgressDialog::updatePercent);
	connect(job, &QProcess::finished, this, &ProgressDialog::jobDone);
	connect(job, &QProcess::errorOccurred, this, &ProgressDialog::jobFailed);
	connect(job, &QProcess::started, this, &ProgressDialog::jobStarted);
	joblist << job;
}

void ProgressDialog::buildlist()
{
#ifdef WIN32
	QString null("NUL");
#else
	QString null("/dev/null");
#endif
	QString base("qonvertmp"),
		vf(" -vf scale=176:-10,expand=176:144,harddup "),
		xvo(" bitrate=80:profile=sp2:chroma_opt:lumi_mask"),
		af(" -af lavcresample=8000,channels=1 "),
		silent(" -msglevel all=0:avsync=5 ");

		if(video && cartoon)
			xvo += ":cartoon";

		if(dvdrip) {
			addjob(new JobVideoEnc("DVD Rip",
				"mencoder -dvd-device \"" + infile + "\" -alang it dvd:// -oac pcm " + af + " -ovc lavc -lavcopts vcodec=ffvhuff -cache 8192 -ofps 12.5 -o " + base + ".avi " + vf + silent, info, this));
			infile = base + ".avi";
		}

		addjob(new JobAnalyse("Analysing file",
				"mplayer -identify -frames 0 -vc null -ac null -vo null -vc null \"" + infile + "\"", info, this));

		if(audio)
			addjob(new JobAudioRip("Audio Rip",
				"mplayer -benchmark \"" + infile + "\" -vo null -vc null -ao pcm:fast:file=" + base + ".wav " + (dvdrip ? "" : af) + silent, info, this));

		if(audio && norm)
			addjob(new JobAudioNorm("Normalizing Audio",
				"normalize-audio --peak " + base + ".wav", info, this, QProcess::StandardError));

		if(audio)
			addjob(new JobAudioEnc("Encoding Audio",
				"amrenc " + base + ".wav " + base + ".amr", info, this, QProcess::StandardError));

		if(video)
			addjob(new JobVideoEnc("Encoding Video (1st pass)",
				"mencoder \"" + infile + "\"" + (dvdrip ? " -vf harddup " : vf) + " -ofps 12.5 -nosound -o " + null + " -of rawvideo -ovc xvid -xvidencopts " + xvo + ":pass=1:turbo -passlogfile " + base + ".log " + silent, info, this));

		if(video)
			addjob(new JobVideoEnc("Encoding Video (2nd pass)",
				"mencoder \"" + infile + "\"" + (dvdrip ? " -vf harddup " : vf) + " -ofps 12.5 -nosound -o " + base + ".m4v -of rawvideo -ovc xvid -xvidencopts " + xvo + ":pass=2 -passlogfile " + base + ".log " + silent, info, this));

		addjob(new JobMux("Muxing Audio and Video",
				"MP4Box -new -3gp -fps 12.5 " + (video ? "-add " + base + ".m4v " : "") + (audio ? "-add " + base + ".amr" : "") + " \"" + outfile + "\"", info, this));
}

void ProgressDialog::jobDone(int result, QProcess::ExitStatus exitstatus)
{
//	qDebug("ProgressDialog::jobDone(%d, %d)", result, exitstatus);

	if(result || exitstatus == QProcess::CrashExit) {
		error();
		return;
	}

	if(jobs->count())
		jobs->item(jobs->count() - 1)->setIcon(doneico);

	if(!joblist.count()) {
		currentp->setValue(100);
		totalp->setValue(100);
		QMessageBox::information(this, "Done", "Encoding done");
		accept();
		return;
	}
	step++;
	currentjob = joblist.takeFirst();
	currentjob->run();
}

void ProgressDialog::jobStarted()
{
//	qDebug("ProgressDialog::jobStarted(%s)", qPrintable(currentjob->title));
	jobs->addItem(new QListWidgetItem(converting, currentjob->title, jobs));
	percentt = step * 100 / steps;
}

void Job::run()
{
	emit gotPercent(0);

	qDebug("%s", qPrintable(args));
	QStringList argv = QProcess::splitCommand(args);
	QString program = argv.takeFirst();
	if(outputchannel == StandardOutput) {
		connect(this, &QProcess::readyReadStandardOutput, this, &Job::readStatus);
		start(program, argv, ReadOnly);
		setReadChannel(StandardOutput);
		closeReadChannel(StandardError);
	} else {
		connect(this, &QProcess::readyReadStandardError, this, &Job::readStatus);
		start(program, argv, ReadOnly);
		closeReadChannel(StandardOutput);
		setReadChannel(StandardError);
	}
	closeWriteChannel();
}

// the first group captured by re in s, or a null string
static QString capture(const QString &s, const char *re)
{
	QRegularExpressionMatch m = QRegularExpression(re).match(s);
	return m.hasMatch() ? m.captured(1) : QString();
}

void JobAnalyse::readStatus()
{
	QString outputline(readAll());
	QString v;
	if(outputline.contains("ID_AUDIO"))
		info.audio = true;
	if(outputline.contains("ID_VIDEO"))
		info.video = true;
	if(!(v = capture(outputline, "ID_LENGTH=(\\d+)")).isNull())
		info.duration = v.toInt();
	if(!(v = capture(outputline, "ID_VIDEO_WIDTH=(\\d+)")).isNull())
		info.resx = v.toInt();
	if(!(v = capture(outputline, "ID_VIDEO_HEIGHT=(\\d+)")).isNull())
		info.resy = v.toInt();
	if(!(v = capture(outputline, "ID_VIDEO_FPS=(\\d+\\.\\d+)")).isNull())
		info.fps = v.toFloat();
	emit gotPercent(100);
}

void JobAudioRip::readStatus()
{
	QString outputline(readAll());
	QString v = capture(outputline, "A: *(\\d+)");
	if(!v.isNull()) {
		int newpercent = v.toInt() * 100 / info.duration;
		if(percent != newpercent)
			emit gotPercent(percent = newpercent);
	}
}

void JobAudioNorm::readStatus()
{
	QString outputline(readAll());
	if(outputline.contains("Applying adjustment"))
		adj = true;
	QString v = capture(outputline, "(\\d+)%");
	if(!v.isNull()) {
		int newpercent = v.toInt() / 3;
		if(adj)
			newpercent = newpercent * 2 + 33;
		if(percent != newpercent)
			emit gotPercent(percent = newpercent);
	}
}

void JobAudioEnc::readStatus()
{
	QString outputline(readAll());
	QString v = capture(outputline, "\\[(\\d+)%\\]");
	if(!v.isNull()) {
		int newpercent = v.toInt();
		if(percent != newpercent)
			emit gotPercent(percent = newpercent);
	}
}

void JobVideoEnc::readStatus()
{
	QString outputline(readAll());
	QString v = capture(outputline, "\\(( ?\\d+)%\\)");
	if(!v.isNull()) {
		int newpercent = v.toInt();
		if(percent != newpercent)
			emit gotPercent(percent = newpercent);
	}
}

void JobMux::readStatus()
{
	int newpercent;
	QString outputline(readAll());
	if(outputline.contains("Importing M4V:"))
		newpercent = outputline.mid(39, 2).toInt() / steps;
	else if(outputline.contains("Importing AMR:"))
		newpercent = outputline.mid(39, 2).toInt() / steps + (100 / steps);
	else if(outputline.contains("ISO File Writing:"))
		newpercent = outputline.mid(42, 2).toInt() / steps + (100 / steps * 2);
	else
		return;
	if(percent != newpercent)
		emit gotPercent(percent = newpercent);
}
