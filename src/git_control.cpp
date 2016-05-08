#include "git_control.h"

#include <iostream>
#include <cassert>
#include <sstream>
#include <string>

#include <QDebug>
#include <QMessageBox>
#include <QDir>

#include "def.h"

#include "git_cmd/git_command.h"
#include "git_cmd/git_clone.h"
#include "git_cmd/git_status.h"
#include "git_cmd/git_init.h"
#include "git_cmd/git_rev_parse.h"
#include "git_cmd/git_pull.h"
#include "git_cmd/git_push.h"

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
	if( num > 0 )
	{
		emit progress_signal(num);
	}
}


	
/*******************************************************************
	set_ui_dynamic_output_func
********************************************************************/
void	GitControl::set_ui_dynamic_output( QList<QByteArray> output_list )
{
	emit output_signal(output_list);
}




/*******************************************************************
	check_folder_exist
********************************************************************/
bool	GitControl::check_folder_exist( std::string path )
{
	QDir	dir( path.c_str() );
	return	dir.exists();
}



/*******************************************************************
	check_git_repo_exist
********************************************************************/
bool	GitControl::check_git_repo_exist( std::string path )
{
	GitRevParse		git_rev_parse;

	QDir	dir1(path.c_str());
	QDir	dir2(git_rev_parse.get_root_path(QString(path.c_str())));

	return	dir1 == dir2;
}




/*******************************************************************
	add
********************************************************************/
void	GitControl::add( QString root_path, QList<QString> str_list )
{
	QProcess		*proc	=	new QProcess(this);
	QStringList		args;

	bool	result	=	false;

	proc->setWorkingDirectory( root_path );

	args << "add";
	args << str_list;

	proc->start( "git", args );

	if( proc->waitForFinished() )
		result	=	true;
	else
		result	=	false;

	if( result == false )
		ERRLOG( "git add fail.");

	delete	proc;	
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
			git_cmd		=	new GitClone( this );
			git_cmd->set_progress_func			=	boost::bind( &GitControl::set_progress, this, _1 );
			git_cmd->input_user_passwd_func		=	boost::bind( &GitControl::input_user_passwd, this );
			connect(	(GitClone*)git_cmd,		SIGNAL(update_proj_button_signal(DbProj)),		this->parent()->parent(),		SLOT(update_proj_button_slot(DbProj))	);
			break;
		case GIT_CMD_PULL:
			git_cmd		=	new GitPull( this );
			break;
		case GIT_CMD_PUSH:
			git_cmd		=	new GitPush( this );
			git_cmd->input_user_passwd_func		=	boost::bind( &GitControl::input_user_passwd, this );
			break;
		default:
			assert(0);
	}

	if( git_cmd == NULL )
		assert(0);
	else
	{
		connect(	git_cmd,	SIGNAL(finished_signal()),		this,			SLOT(cmd_finished_slot())		);
		connect(	this,		SIGNAL(abort_signal()),			git_cmd,		SLOT(abort_slot())				);

		git_cmd->set_ui_dynamic_output_func	=	boost::bind( &GitControl::set_ui_dynamic_output, this, _1 );
		git_cmd->exec( param );
	}
}



/*******************************************************************
	push
********************************************************************/
void	GitControl::push( QString root_path )
{
	GitParameter	param;
	param.insert( make_pair( GIT_PUSH_ROOT_PATH, root_path ) );
	exec_command( GIT_CMD_PUSH, param );
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
	check_exist_git_repository
********************************************************************/
QString		GitControl::check_exist_git_repository( QString path )
{
	GitRevParse		git_rev_parse;
	QString			root_path	=	git_rev_parse.get_root_path(path);
    
    /*int     size    =   root_path.size();
    if( size > 0 )
    {
        if( root_path[size-1] == '\n')
            root_path.remove(size-1,1);
    }*/

	return	root_path;
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
	return	GitCommand::get_proj_name(path);
}



/*******************************************************************
	init
********************************************************************/
bool	GitControl::init( QString path )
{
    GitInit     git_init;
    bool	    result  =   git_init.exec(path);

    return	result;
}



/*******************************************************************
	pull
********************************************************************/
void	GitControl::pull( QString root_path )
{
	GitParameter	param;
	param.insert( make_pair( GIT_PULL_ROOT_PATH, root_path ) );
	exec_command( GIT_CMD_PULL, param );
}


/*******************************************************************
	clone
********************************************************************/
void	GitControl::clone( QString src, QString dest, bool is_local_path, QString username, QString password )
{
	GitParameter	param;

	param.insert( make_pair( GIT_CLONE_SOURCE,		src ) );
	param.insert( make_pair( GIT_CLONE_DESTINATION, dest ) );

	if( username.isEmpty() == false )
		param.insert( make_pair( GIT_CLONE_USERNAME,	username ) );
	if( password.isEmpty() == false )
		param.insert( make_pair( GIT_CLONE_PASSWORD,	password ) );

	if( is_local_path == true )
		param.insert( make_pair( GIT_CLONE_LOCAL, QString("true") ) );
	else
		param.insert( make_pair( GIT_CLONE_LOCAL, QString("false") ) );

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




