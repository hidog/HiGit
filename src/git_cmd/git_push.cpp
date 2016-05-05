#include "git_push.h"

#include <QDebug>
#include "../def.h"



/*******************************************************************
	GitPush
********************************************************************/
GitPush::GitPush( QObject *parent ) :
	GitCommand(parent)
{}



/*******************************************************************
	GitPush
********************************************************************/
GitPush::~GitPush()
{}



/*******************************************************************
	exec
********************************************************************/
void	GitPush::exec( GitParameter param )
{
	QProcess		*proc	=	new QProcess(this);
	QStringList		args;
	QString			root_path;

	if( param.find( GIT_PUSH_ROOT_PATH ) != param.end() )
		root_path	=	param[GIT_PUSH_ROOT_PATH];
	else
		ERRLOG("root path not found.")

	proc->setWorkingDirectory(root_path);
	
	args << "push";
	args << "-v";
	args << "--progress";

	// init data.
	output_list.clear();
	last_msg.clear();
	remain_msg.clear();
	
	connect(	proc,	SIGNAL(readyReadStandardError()),				this,			SLOT(push_output_err_slot())						);
	connect(	proc,	SIGNAL(readyReadStandardOutput()),				this,			SLOT(push_output_std_slot())						);
	connect(	proc,	SIGNAL(readyRead()),							this,			SLOT(push_output_slot())							);
	connect(	proc,	SIGNAL(finished(int,QProcess::ExitStatus)),		this,			SLOT(push_finish_slot(int,QProcess::ExitStatus))	);
	connect(	proc,	SIGNAL(started()),								this,			SLOT(push_start_slot())								);
	connect(	proc,	SIGNAL(error(QProcess::ProcessError)),			this,			SLOT(push_error_slot(QProcess::ProcessError))		);
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
	push_output_slot
********************************************************************/
void	GitPush::push_finish_slot( int, QProcess::ExitStatus )
{
	printf("test");
}


/*******************************************************************
	push_output_slot
********************************************************************/
void	GitPush::push_start_slot()
{
	printf("test");
}

/*******************************************************************
	push_output_slot
********************************************************************/
void	GitPush::push_error_slot( QProcess::ProcessError )
{
	printf("test");
}



/*******************************************************************
	push_output_slot
********************************************************************/
void	GitPush::push_output_slot()
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
			//splite_progress( data, msg, num );
			//set_progess( num );
			//refresh_dynamic_output( data, msg );
		}
		else		
			break;	// means not get enough string yet.
	}
}



/*******************************************************************
	push_output_std_slot
********************************************************************/
void	GitPush::push_output_std_slot()
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
			//splite_progress( data, msg, num );
			//set_progess( num );
			//refresh_dynamic_output( data, msg );
		}
		else		
			break;	// means not get enough string yet.
	}
}


/*******************************************************************
	push_output_err_slot
********************************************************************/
void	GitPush::push_output_err_slot()
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
			//splite_progress( data, msg, num );
			//set_progess( num );
			//refresh_dynamic_output( data, msg );
		}
		else		
			break;	// means not get enough string yet.
	}
}