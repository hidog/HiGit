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
