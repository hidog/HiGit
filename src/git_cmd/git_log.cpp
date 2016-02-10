#include "git_log.h"	

#include <QDebug>


/*******************************************************************
	GitLog
********************************************************************/
GitLog::GitLog( QObject *parent ) :
	GitCommand(parent)
{}



/*******************************************************************
	GitLog
********************************************************************/
GitLog::~GitLog()
{}

	



/*******************************************************************
	exec
********************************************************************/
void	GitLog::exec( GitParameter param )
{}




/*******************************************************************
	test
********************************************************************/
void	GitLog::test( QString path )
{
	QProcess		*proc	=	new QProcess(this);
	QStringList		args;

	proc->setWorkingDirectory( path );

	args << "log";

	proc->start( "git", args );

	bool	res		=	proc->waitForFinished();

	if( res )
	{
		QByteArray	output	=	proc->readAll();
		//qDebug() << output;

		while(output.length() > 0 )
		{
			QByteArray	str		=	splite_git_output(output);
			qDebug(str);
		}
	}


	delete	proc;
}
