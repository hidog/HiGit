#include "git_control.h"

#include <iostream>
#include <cassert>
#include <sstream>
#include <string>

#include <QDebug>
#include <QMessageBox>

#include "def.h"

#include "git_cmd/git_command.h"
#include "git_cmd/git_clone.h"

#ifndef Q_MOC_RUN
#	include<boost/bind.hpp>
#endif

using  namespace std;



/*******************************************************************
	GitControl
********************************************************************/
GitControl::GitControl( QWidget *parent )
	:	QObject(parent)
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
void	GitControl::set_progress( int num )
{
	emit progress_signal(num);
}


	
/*******************************************************************
	set_ui_dynamic_output_func
********************************************************************/
void	GitControl::set_ui_dynamic_output( QList<QByteArray> output_list )
{
	emit output_signal(output_list);
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
	{
		connect(	git_cmd,	SIGNAL(finished_signal()),	this,		SLOT(cmd_finished_slot())	);
		connect(	this,		SIGNAL(abort_signal()),		git_cmd,	SLOT(abort_slot())			);

		git_cmd->set_progress_func			=	boost::bind( &GitControl::set_progress, this, _1 );
		git_cmd->set_ui_dynamic_output_func	=	boost::bind( &GitControl::set_ui_dynamic_output, this, _1 );
		git_cmd->input_user_passwd_func		=	boost::bind( &GitControl::input_user_passwd, this );

		git_cmd->exec( param );
	}
}



/*******************************************************************
	input_user_passwd
********************************************************************/
void	GitControl::input_user_passwd()
{
	emit need_user_pw_signal();
}




/*******************************************************************
	abort_cmd
********************************************************************/
void	GitControl::abort_cmd()
{
	emit abort_signal();
}



/*******************************************************************
	cmd_finished_slot
********************************************************************/
void		GitControl::cmd_finished_slot()
{
	GitCommand	*git_cmd	=	(GitCommand*)sender();
	delete	git_cmd;

	emit cmd_finished_signal();
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
	get_proj_name
********************************************************************/
QString		GitControl::get_proj_name( QString path )
{
#ifdef _WIN32
	int		index	=	path.lastIndexOf( '/' );
	return	path.mid( index+1 );
#else
#error need maintain.
#endif
}



/*******************************************************************
	init
********************************************************************/
bool	GitControl::init( QString path )
{
	QProcess		*proc	=	new QProcess(this);
	QStringList		args;

	bool	result;

	args << "init";
	args << path;

	proc->start( "git", args );

	result	=	proc->waitForFinished();
	QByteArray	output	=	proc->readAll();
	qDebug(output);

	if( result )
		QMessageBox::about( NULL, "init", "init success." );
	else
		QMessageBox::critical( NULL, "init", "init fail." );		

	delete	proc;

	return	result;
}





/*******************************************************************
	clone
********************************************************************/
#if 0
void	GitControl::clone( QString src, QString dest )
{
	GitParameter	param;

	param.insert( make_pair( GIT_CLONE_SOURCE,		src ) );
	param.insert( make_pair( GIT_CLONE_DESTINATION, dest ) );

	if( get_recursive_state_func() == true )
		param.insert( make_pair( GIT_CLONE_RECURSIVE, QString("true") ) );

	exec_command( GIT_CMD_CLONE, param );
}
#endif

/*******************************************************************
	clone
********************************************************************/
void	GitControl::clone( QString src, QString dest, QString username, QString password )
{
	GitParameter	param;

	param.insert( make_pair( GIT_CLONE_SOURCE,		src ) );
	param.insert( make_pair( GIT_CLONE_DESTINATION, dest ) );

	if( username.isEmpty() == false )
		param.insert( make_pair( GIT_CLONE_USERNAME,	username ) );
	if( password.isEmpty() == false )
		param.insert( make_pair( GIT_CLONE_PASSWORD,	password ) );

	if( get_recursive_state_func() == true )
		param.insert( make_pair( GIT_CLONE_RECURSIVE, QString("true") ) );

	if( get_depth_state_func() == true )
		param.insert( make_pair( GIT_CLONE_DEPTH, QString("%1").arg(get_depth_num_func()) ) );

	exec_command( GIT_CMD_CLONE, param );
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




