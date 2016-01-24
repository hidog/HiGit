#include "git_control.h"

#include <iostream>
#include <cassert>
#include <sstream>
#include <string>

#include <QDebug>
#include <QMessageBox>

#include "def.h"

#include "git_command.h"
#include "git_clone.h"

using  namespace std;



/*******************************************************************
	GitControl
********************************************************************/
GitControl::GitControl( QWidget *parent )
	:	QObject(parent), last_index(0)
{
	memset( msg_buf, 0, GIT_BUF_SIZE );	
	set_connect();
	remain_msg.clear();
}


/*******************************************************************
	GitControl
********************************************************************/
GitControl::~GitControl()
{
}



/*******************************************************************
	exec_command
********************************************************************/
void	GitControl::exec_command( GIT_COMMAND_TYPE cmd_type, GitParameter param )
{
	GitCommand		*git_cmd	=	NULL;

	switch( cmd_type )
	{
		case GIT_CMD_CLONE:
			git_cmd		=	new GitClone();
			break;
		default:
			assert(0);
	}

	if( git_cmd == NULL )
		assert(0);
	else
		git_cmd->exec( param );

	// need delete
	delete	git_cmd;
	git_cmd		=	NULL;
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
	用版本來檢查
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
#if 0

void		GitControl::clone( QString src, QString dest, QString username, QString password )
{
	#if 0
	QProcess		*proc	=	new QProcess(this);
	QStringList		args;
	int				index;
	
	// 初始化資料 
	remain_msg.clear();

	args << "clone";
	args << "-v";
	args << "--progress";

	if( get_recursive_state_func() == true )
		args << "--recursive";

	index	=	src.indexOf("://");
	src.insert( index+3, QString("%1:%2@").arg(username).arg(password) );
	//qDebug() << src;
	args << src;
	args << dest;

	// init data.
	last_index	=	0;
	output_list.clear();
	memset( msg_buf, 0, GIT_BUF_SIZE );
	
	connect(	proc,	SIGNAL(readyReadStandardError()),				this,	SLOT(clone_output_err_slot())						);
	connect(	proc,	SIGNAL(readyReadStandardOutput()),				this,	SLOT(clone_output_std_slot())						);
	connect(	proc,	SIGNAL(readyRead()),							this,	SLOT(clone_output_slot())							);
	connect(	proc,	SIGNAL(finished(int,QProcess::ExitStatus)),		this,	SLOT(clone_finish_slot(int,QProcess::ExitStatus))	);
	connect(	proc,	SIGNAL(started()),								this,	SLOT(clone_start_slot())							);
	connect(	proc,	SIGNAL(error(QProcess::ProcessError)),			this,	SLOT(clone_error_slot(QProcess::ProcessError))		);

	/*
		note: git clone will create thread, so need set process channel for get output in other thread.
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
#endif
}
#endif

/*******************************************************************
	clone
********************************************************************/
void	GitControl::clone( QString src, QString dest )
{
	GitParameter	param;

	param.insert( make_pair( GIT_CLONE_SOURCE,		src ) );
	param.insert( make_pair( GIT_CLONE_DESTINATION, dest ) );

	if( get_recursive_state_func() == true )
		param.insert( make_pair( GIT_CLONE_RECURSIVE, QString("true") ) );

	exec_command( GIT_CMD_CLONE, param );
}




/*******************************************************************
	set_color
********************************************************************/
void	GitControl::set_color( QByteArray& data, GIT_FONT_COLOR color )
{
	switch( color )
	{
		case GIT_FONT_RED:
			data.push_front("<font color=\"red\">");	
			data.push_back("</font>");
			break;
		case GIT_FONT_BLUE:
			data.push_front("<font color=\"blue\">");	
			data.push_back("</font>");
			break;
		default:
			assert(0);
	}
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




