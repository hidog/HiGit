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
		connect(	git_cmd,	SIGNAL(finished_signal()),	this,	SLOT(cmd_finished_slot())	);

		git_cmd->set_progress_func			=	boost::bind( &GitControl::set_progress, this, _1 );
		git_cmd->set_ui_dynamic_output_func	=	boost::bind( &GitControl::set_ui_dynamic_output, this, _1 );

		git_cmd->exec( param );
	}

	// need delete
	//delete	git_cmd;
	//git_cmd		=	NULL;
}



/*******************************************************************
	cmd_finished_slot
********************************************************************/
void		GitControl::cmd_finished_slot()
{
	GitCommand	*git_cmd	=	(GitCommand*)sender();

	delete	git_cmd;
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




