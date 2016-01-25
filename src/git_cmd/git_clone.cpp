#include "git_clone.h"
#include "../def.h"

#include <QProcess>
#include <QDebug>


/*******************************************************************
	GitClone
********************************************************************/
GitClone::GitClone( QWidget *parent )
    :   GitCommand(parent)
{}


/*******************************************************************
	GitClone
********************************************************************/
GitClone::~GitClone()
{}




/*******************************************************************
	exec
********************************************************************/
void	GitClone::exec( GitParameter param )
{
	QProcess		*proc	=	new QProcess(this);
	QStringList		args;
	QString			src		=	param[GIT_CLONE_SOURCE];
	QString			dest	=	param[GIT_CLONE_DESTINATION];
	QString			fixed_src;
	
	args << "clone";
	args << "-v";
	args << "--progress";

	if( param.find(GIT_CLONE_RECURSIVE) != param.end() )
		args << "--recusive";

	if( param.find(GIT_CLONE_DEPTH) != param.end() )
		args << QString("--depth") << param[GIT_CLONE_DEPTH];

	for( int i = 0; i < args.size(); i++ )
		qDebug() << args[i];

	// get username, password, project name.
	parse_host( src, type, host, username, password, port, content );

	if( param.find(GIT_CLONE_PASSWORD) != param.end() )
	{
		username	=	param[GIT_CLONE_USERNAME];
		password	=	param[GIT_CLONE_PASSWORD];
		// replace username, password.
		fixed_src	=	QString("%1://%2:%3@%4").arg(type).arg(username).arg(password).arg(host);
	}
	else
		fixed_src	=	src;

	args << fixed_src;
	args << dest;	

	// init data.
	output_list.clear();
	last_msg.clear();
	remain_msg.clear();
	
	connect(	proc,	SIGNAL(readyReadStandardError()),				this,	SLOT(clone_output_err_slot())						);
	connect(	proc,	SIGNAL(readyReadStandardOutput()),				this,	SLOT(clone_output_std_slot())						);
	connect(	proc,	SIGNAL(readyRead()),							this,	SLOT(clone_output_slot())							);
	connect(	proc,	SIGNAL(finished(int,QProcess::ExitStatus)),		this,	SLOT(clone_finish_slot(int,QProcess::ExitStatus))	);
	connect(	proc,	SIGNAL(started()),								this,	SLOT(clone_start_slot())							);
	connect(	proc,	SIGNAL(error(QProcess::ProcessError)),			this,	SLOT(clone_error_slot(QProcess::ProcessError))		);
	connect(	this,	SIGNAL(abort_signal()),							proc,	SLOT(kill())										);

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
	clone_error_slot
********************************************************************/
void	GitClone::clone_error_slot( QProcess::ProcessError err )
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



/*******************************************************************
	clone_finish_slot
********************************************************************/
void	GitClone::clone_finish_slot( int exit_code, QProcess::ExitStatus exit_status )
{
	// delete proc
	QProcess	*proc	=	(QProcess*)sender();
	delete		proc;

	QByteArray	output;

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
	abort_slot
********************************************************************/
void	GitClone::abort_slot()
{
	qDebug() << "GitClone::abort_slot()";
	emit abort_signal();
}



/*******************************************************************
	clone_start_slot
********************************************************************/
void	GitClone::clone_start_slot()
{
	qDebug() << "git clone start...";

	QProcess	*proc	=	(QProcess*)sender();
	//proc->write(".");

	QByteArray	output	=	proc->readAll();
	qDebug() << output ;

}


/*******************************************************************
	clone_output_slot
********************************************************************/
void	GitClone::clone_output_slot()
{
	QProcess	*proc	=	(QProcess*)sender();
	QByteArray	output	=	proc->readAll();	

	qDebug() << output;
	//qDebug() << output << "end...";
	//emit( output_signal(output) );
}






/*******************************************************************
	clone_output_slot
********************************************************************/
void	GitClone::clone_output_err_slot()
{
	QProcess	*proc	=	(QProcess*)sender();
	QByteArray	output	=	proc->readAllStandardError();

	QByteArray	data;		// raw data, contain %. like "Receive 10%"
	QByteArray	msg;		// splite %, like Receive

	int		i;
	int		num;

	// get string this term. keep some string in remain_str if needed.
	splite_remain( output );

	while( output.length() > 0 )
	{
		data	=	splite_git_output( output );

		if( data.length() > 0 )
		{
			if( need_password( data ) == true )
			{
				proc->kill();
				input_user_passwd_func();
			}
			else
			{
				splite_progress( data, msg, num );;
				set_progess( num );
				refresh_dynamic_output( data, msg );
			}
		}
		else		
			break;	// means not get enough string yet.
	}
}


/*******************************************************************
	clone_output_slot
********************************************************************/
void	GitClone::clone_output_std_slot()
{
	QProcess	*proc	=	(QProcess*)sender();
	QByteArray	output	=	proc->readAllStandardOutput();

	//cout << "std... " << qPrintable( output ) << " ... end output\n";
	//emit( output_signal(output) );

	qDebug() << output;
}

