#include "git_status.h"

#include <QProcess>
#include "../def.h"
#include <QDebug>
#include <QColor>


/*******************************************************************
	GitStatus
********************************************************************/
GitStatus::GitStatus( QObject *parent ) :
	GitBase(parent)
{}


/*******************************************************************
	GitStatus
********************************************************************/
GitStatus::~GitStatus()
{}








/*******************************************************************
	get_delete_files
********************************************************************/
QStringList		GitStatus::get_delete_files( QString path )
{
	QProcess		*proc	=	new QProcess();
	QStringList		args;

	//qDebug() << path << " " << filename;

	proc->setWorkingDirectory( path );
	args << "status";
	proc->start( "git", args );    

	bool	result;
	result	=	proc->waitForFinished();

	QString			status;
	QByteArray		output,	str;
	int				index;
	QRegExp			rexp( "(\\S+)" );
	QString			filename;

	QStringList		list;

	if( result == true )
	{
		output	=	proc->readAll();
		remain_msg	=	"";

		while( output.length() > 0 )
		{
			str		=	splite_git_output( output );
			if( str.contains("deleted:") )
			{
				//qDebug() << str;
				index	=	str.indexOf(':');
				if( rexp.indexIn( str, index+1 ) != -1 )
				{
					filename	=	rexp.cap(1);
					index		=	filename.lastIndexOf('/');

					//qDebug() << filename;
#if 0
					// 移除folder的sample code.
					if( index >= 0 )
						filename.remove( 0, index+1 );
#endif

					//qDebug() << filename;
					// 同目錄的才顯示.
					if( index < 0 )
						list << filename;
				}
				
			}
		}	
	}
	else
		status	=	QString("");

	delete	proc;
	return	list;
}



/*******************************************************************
	get_file_color
********************************************************************/
QColor		GitStatus::get_file_color( QString path, QString filename )
{
	QString		status	=	get_file_status( path, filename );
	QColor		color	=	get_status_color( status );

	/*if( status == "tracked" )
		color	=	QColor(Qt::darkGreen);
	else if( status == "added" )
		color	=	QColor(Qt::blue);
	else if( status == "modified" )
		color	=	QColor(Qt::red);*/

	return	color;
}


/*******************************************************************
	get_file_color
********************************************************************/
QColor		GitStatus::get_status_color( QString status )
{
	QColor		color	=	QColor();

	if( status == "tracked" )
		color	=	QColor(Qt::darkGreen);
	else if( status == "added" )
		color	=	QColor(Qt::blue);
	else if( status == "modified" )
		color	=	QColor(Qt::red);

	return	color;
}




/*******************************************************************
	get_file_status
********************************************************************/
QString		GitStatus::get_file_status( QString path, QString filename )
{
	QProcess		*proc	=	new QProcess();
	QStringList		args;

	//qDebug() << path << " " << filename;

	proc->setWorkingDirectory( path );

	args << "status";
	args << filename;

	proc->start( "git", args );    

	bool	result;
	result	=	proc->waitForFinished();

	QString		status;

	if( result == true )
	{
		QByteArray	output	=	proc->readAll();

		//qDebug() << output;

		if( output.contains("nothing to commit, working directory clean") == true )
			status	=	"tracked";

		else if( output.contains("modified:") == true )
			status	=	"modified";

		else if( output.contains("new file:") == true )
			status	=	"added";

		else if( output.contains("Untracked files:") == true )
			status	=	"untracked";

		else if( output.contains("deleted:") == true )
			status	=	"deleted";

		else
			ERRLOG("error. output = %s", qPrintable(output) )
		
	}
	else
		status	=	QString("");

	delete	proc;
	return	status;
}
