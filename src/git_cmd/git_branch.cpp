#include "git_branch.h"

#include "../def.h"



/*******************************************************************
	GitBranch
********************************************************************/
GitBranch::GitBranch( QString _root_path, QObject *parent ) :
	root_path(_root_path), GitBase(parent)
{}





/*******************************************************************
	GitBranch
********************************************************************/
GitBranch::~GitBranch()
{}


/*******************************************************************
	get_all_branch
	set current branch at the first.
********************************************************************/
QStringList		GitBranch::get_all_branch()
{
	QProcess		*proc	=	new QProcess(this);
	QStringList		args;
	QByteArray		output;
	QByteArray		str;
	QStringList		list;

	proc->setWorkingDirectory(root_path);
	args << "branch";

	proc->start( "git", args );

	if( proc->waitForFinished() )
		output	=	proc->readAll();
	else
		ERRLOG("git branch fail.")

	// parse output.
	while( output.length() > 0 )
	{
		str		=	splite_git_output( output );

		if( str.length() > 3 )
		{
			if( str[0] == '*' )
				list.push_front( QString(str.mid(2)) );
			else
				list.push_back( QString(str.mid(2)) );
		}
	}

	delete proc;
	return	list;
}



/*******************************************************************
	get_remote_branc
********************************************************************/
QStringList		GitBranch::get_remote_branc()
{
	QProcess		*proc	=	new QProcess(this);
	QStringList		args;
	QByteArray		output;
	QByteArray		str;
	QStringList		list;

	int		index;

	proc->setWorkingDirectory(root_path);
	args << "branch" << "--remote";

	proc->start( "git", args );

	if( proc->waitForFinished() )
		output	=	proc->readAll();
	else
		ERRLOG("git branch fail.")

	// parse output.
	while( output.length() > 0 )
	{
		str		=	splite_git_output( output );

		if( str.length() > 3 )
		{
			if( str.contains("->") == false )
			{
				index	=	str.indexOf("/");
				list.push_back( str.mid(index+1) );
			}
		}
	}

	delete	proc;
	return	list;
}



/*******************************************************************
	current_branch
********************************************************************/
QString		GitBranch::current_branch()
{
	QProcess		*proc	=	new QProcess(this);
	QStringList		args;
	QByteArray		output;
	QByteArray		str;
	QString			branch_name;

	proc->setWorkingDirectory(root_path);
	args << "branch";

	proc->start( "git", args );

	if( proc->waitForFinished() )
		output	=	proc->readAll();
	else
		ERRLOG("git branch fail.")

	// parse output.
	while( output.length() > 0 )
	{
		str		=	splite_git_output( output );

		if( str.length() > 3 )
		{
			if( str[0] == '*' )
			{
				branch_name	=	str.mid( 2 );
				break;
			}
		}
	}

	delete proc;
	return	branch_name;
}
