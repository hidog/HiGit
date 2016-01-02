#include "git_control.h"

#include <iostream>
#include <cassert>
#include <sstream>
#include <string>

#include <QDebug>
#include <QMessageBox>

#include "def.h"

using  namespace std;



/*******************************************************************
	GitControl
********************************************************************/
GitControl::GitControl()
	:	QObject()
{
	set_connect();
}


/*******************************************************************
	GitControl
********************************************************************/
GitControl::~GitControl()
{
}


/*******************************************************************
	GitControl
********************************************************************/
void	GitControl::set_connect()
{
	//connect(	proc,	SIGNAL(readyRead()),					this,	SLOT(on_read())								);
	//connect(	proc,	SIGNAL(error(QProcess::ProcessError)),	this,	SLOT(error_slots(QProcess::ProcessError))	);
}




/*******************************************************************
	check_git_exist
********************************************************************/
bool	GitControl::check_git_exist()
{
	QProcess		*proc	=	new QProcess(this);
	QStringList		args;

	bool	result;

	args << "--version";
	proc->start( "git", args );

	if( proc->waitForFinished() )
		result	=	true;
	else
		result	=	false;

	delete	proc;
	return	result;
}




/*******************************************************************
	init
********************************************************************/
void	GitControl::init( QString path )
{
	QProcess		*proc	=	new QProcess(this);
	QStringList		args;

	args << "init";
	args << path;

	proc->start( "git", args );

	if( proc->waitForFinished() )
		QMessageBox::about( NULL, "init", "init success." );
	else
		QMessageBox::critical( NULL, "init", "init fail." );		

	delete	proc;
}




/*******************************************************************
	clone
********************************************************************/
void	GitControl::clone( QString src, QString dest )
{
	QProcess		*proc	=	new QProcess(this);
	QStringList		args;
	
	args << "clone";
	args << src;
	args << dest;
	
	connect(	proc,	SIGNAL(readyReadStandardError()),				this,	SLOT(clone_output_slot())							);
	connect(	proc,	SIGNAL(finished(int,QProcess::ExitStatus)),		this,	SLOT(clone_finish_slot(int,QProcess::ExitStatus))	);
	connect(	proc,	SIGNAL(started()),								this,	SLOT(clone_start_slot())							);
	connect(	proc,	SIGNAL(error(QProcess::ProcessError)),			this,	SLOT(clone_error_slot(QProcess::ProcessError))		);
	
	/*
		note: git clone will create thread, so need set process channel for get output in other thread.
	*/
	proc->setProcessChannelMode( QProcess::ForwardedChannels  );
	proc->start( "git", args );

	switch(proc->state())
	{
		case QProcess::NotRunning:
			PRINT_ENUM(QProcess::NotRunning);
			delete	proc;
			break;
		case QProcess::Starting:
			PRINT_ENUM(QProcess::Starting);
			break;
		case QProcess::Running:
			PRINT_ENUM(QProcess::Running);
			break;
		default:
			assert(0);
	}
}




/*******************************************************************
	clone_error_slot
********************************************************************/
void	GitControl::clone_error_slot( QProcess::ProcessError err )
{
	switch(err)
	{
		case QProcess::FailedToStart:
			PRINT_ENUM(QProcess::FailedToStart);
			break;
		case QProcess::Crashed:
			PRINT_ENUM(QProcess::Crashed);
			break;
		case QProcess::Timedout:
			PRINT_ENUM(QProcess::Timedout);
			break;
		case QProcess::WriteError:
			PRINT_ENUM(QProcess::WriteError);
			break;
		case QProcess::ReadError:
			PRINT_ENUM(QProcess::ReadError);
			break;
		case QProcess::UnknownError:
			PRINT_ENUM(QProcess::UnknownError);
			break;
		default:
			assert(0);
	}
}



/*******************************************************************
	clone_finish_slot
********************************************************************/
void	GitControl::clone_finish_slot( int exit_code, QProcess::ExitStatus exit_status )
{
	// delete proc
	QProcess	*proc	=	(QProcess*)sender();
	delete	proc;

	qDebug() << "exist_code: " << exit_code;
	
	switch(exit_status)
	{
		case QProcess::NormalExit:
			qDebug() << "norma exit.";
			break;
		case QProcess::CrashExit:
			qDebug() << "crash exit.";
			break;
		default:
			assert(0);
	}
}



/*******************************************************************
	clone_start_slot
********************************************************************/
void	GitControl::clone_start_slot()
{
	qDebug() << "git clone start...";
}


/*******************************************************************
	clone_output_slot
********************************************************************/
void	GitControl::clone_output_slot()
{
	QProcess	*proc	=	(QProcess*)sender();
	qDebug() << proc->readAllStandardError();
}



/*******************************************************************
	get_version
********************************************************************/
string		GitControl::get_version()
{
	QProcess		*proc	=	new QProcess(this);
	QStringList		args;
	string			version;
	stringstream	ss;

	args << "--version";
	proc->start( "git", args );

	if( proc->waitForFinished() )
		ss << proc->readAll().data();
	else
		ss << "git not exists.";

	return	version;
}


/*******************************************************************
	error_slots
********************************************************************/
void	GitControl::error_slot( QProcess::ProcessError err )
{
	cout << "QProcess get err = " << err << endl;
}




