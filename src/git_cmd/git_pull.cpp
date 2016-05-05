#include "git_pull.h"

#include "../def.h"
#include <QDebug>
#include <QProcess>

/*******************************************************************
	GitPull
********************************************************************/
GitPull::GitPull( QObject *parent ) :
	GitCommand(parent)
{}





/*******************************************************************
	GitPull
********************************************************************/
GitPull::~GitPull()
{}


/*******************************************************************
	abort_slot
********************************************************************/
void	GitPull::abort_slot()
{
	qDebug() << "need maintain.";
}



/*******************************************************************
	exec
********************************************************************/
void	GitPull::exec( GitParameter param )
{
	QProcess		*proc	=	new QProcess(this);
	QStringList		args;
	QString			root_path;

	if( param.find( GIT_PULL_ROOT_PATH ) != param.end() )
		root_path	=	param[GIT_PULL_ROOT_PATH];
	else
		ERRLOG("root path not found.")

	proc->setWorkingDirectory(root_path);
	
	args << "pull";
	args << "-v";
	args << "--progress";

	// init data.
	output_list.clear();
	last_msg.clear();
	remain_msg.clear();
	
	connect(	proc,	SIGNAL(readyReadStandardError()),				this,			SLOT(pull_output_err_slot())						);
	connect(	proc,	SIGNAL(readyReadStandardOutput()),				this,			SLOT(pull_output_std_slot())						);
	connect(	proc,	SIGNAL(readyRead()),							this,			SLOT(pull_output_slot())							);
	connect(	proc,	SIGNAL(finished(int,QProcess::ExitStatus)),		this,			SLOT(pull_finish_slot(int,QProcess::ExitStatus))	);
	connect(	proc,	SIGNAL(started()),								this,			SLOT(pull_start_slot())								);
	connect(	proc,	SIGNAL(error(QProcess::ProcessError)),			this,			SLOT(pull_error_slot(QProcess::ProcessError))		);
	//connect(	this,	SIGNAL(abort_signal()),							proc,			SLOT(kill())										);

	/*
		note: need use -v, otherwise no message.
	*/
    proc->setProcessChannelMode( QProcess::SeparateChannels );
	proc->start( "git", args, QProcess::ReadWrite );

	switch(proc->state())
	{
		case QProcess::NotRunning:
			PRINT_ENUM(QProcess::NotRunning);
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
	pull_finish_slot
********************************************************************/
void	GitPull::pull_finish_slot( int exit_code, QProcess::ExitStatus exit_status )
{
	// delete proc
	QProcess	*proc	=	(QProcess*)sender();
	delete		proc;


	switch(exit_status)
	{
		case QProcess::NormalExit:
			PRINT_ENUM(QProcess::NormalExit);
			break;
		case QProcess::CrashExit:
			PRINT_ENUM(QProcess::CrashExit);
			break;
		default:
			assert(0);
	}

	emit finished_signal();
}


/*******************************************************************
	pull_output_err_slot
********************************************************************/
void	GitPull::pull_output_err_slot()
{
	// main handle
	QProcess	*proc	=	(QProcess*)sender();
	QByteArray	output	=	proc->readAllStandardError();

	QByteArray	data;		// raw data, contain %. like "Receive 10%"
	QByteArray	msg;		// splite %, like Receive

	int		num;

	// get string this term. keep some string in remain_str if needed.
	splite_remain( output );

	//
	while( output.length() > 0 )
	{
		data	=	splite_git_output( output );
        qDebug() << data;

		if( data.length() > 0 )
		{
			splite_progress( data, msg, num );
			//set_progess( num );
			refresh_dynamic_output( data, msg );
		}
		else		
			break;	// means not get enough string yet.
	}
}


/*******************************************************************
	pull_output_std_slot
********************************************************************/
void	GitPull::pull_output_std_slot()
{
	QProcess	*proc	=	(QProcess*)sender();
	QByteArray	output	=	proc->readAllStandardOutput();

	qDebug() << output;
}


/*******************************************************************
	pull_output_slot
********************************************************************/
void	GitPull::pull_output_slot()
{
	// will output something.
	QProcess	*proc	=	(QProcess*)sender();
	QByteArray	output	=	proc->readAll();	
	QByteArray	data;		// raw data, contain %. like "Receive 10%"
	QByteArray	msg;		// splite %, like Receive

	int		num;

	splite_remain( output );

	//
	while( output.length() > 0 )
	{
		data	=	splite_git_output( output );

		if( data.length() > 0 )
		{
			splite_progress( data, msg, num );;
			//set_progess( num );
			refresh_dynamic_output( data, msg );
		}
		else		
			break;	// means not get enough string yet.
	}
}


/*******************************************************************
	pull_start_slot
********************************************************************/
void	GitPull::pull_start_slot()
{
	qDebug() << "git pull start...";
	//Process	*proc	=	(QProcess*)sender();
}



/*******************************************************************
	pull_error_slot
********************************************************************/
void	GitPull::pull_error_slot( QProcess::ProcessError err )
{
	QByteArray	result;

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