#include "git_status.h"

#include <QProcess>
#include "../def.h"
#include <QDebug>
#include <QColor>
#include <QTextCodec>


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
	parse_short_status
	file:///C:/Users/hidog/AppData/Local/Programs/Git/mingw64/share/doc/git-doc/git-status.html
********************************************************************/
/*void	GitStatus::parse_short_status( FileInfoList &list, const QByteArray &str )
{
	char		X,	Y;	// naming see above URL.
	QString		name;
	FileInfo	info;

	X		=	str[0];
	Y		=	str[1];
	name	=	str.mid( 3 );

	// don't needed handle sub directory files.
	// also do not handle chinese file.
	if( name.contains("/") || name.contains("\\") )
		return;

	//qDebug() << name;

	// decide X
	switch( X )
	{
		case 'A':
			info.name			=	name;
			info.status			=	GIT_STATUS_ADDED;
			info.font_color		=	get_status_color( info.status );
			list.push_back(info);
			return;
	}

	// decide Y
	switch( Y )
	{
		case 'M' :	
			info.name			=	name;
			info.status			=	GIT_STATUS_MODIFY;
			info.font_color		=	get_status_color( info.status );
			list.push_back(info);
			return;

		case 'A' :
			info.name			=	name;
			info.status			=	GIT_STATUS_ADDED;
			info.font_color		=	get_status_color( info.status );
			list.push_back(info);
			return;

		case '?' :
			info.name			=	name;
			info.status			=	GIT_STATUS_UNTRACKED;
			info.font_color		=	get_status_color( info.status );
			list.push_back(info);

		case 'D':
			return;

		default:
			// there are something status not define, see above URL.
			ERRLOG("status fail.")
	}
}*/



/*******************************************************************
	get_all_status
	file:///C:/Users/hidog/AppData/Local/Programs/Git/mingw64/share/doc/git-doc/git-status.html
********************************************************************/
/*
FileInfoList	GitStatus::get_all_status( QString path )
{
	QProcess		*proc	=	new QProcess();
	QStringList		args;
	bool			result;
	FileInfoList	list;
	QByteArray		output;
	QByteArray		str;

	proc->setWorkingDirectory( path );
	args << "status" << "-s";

	proc->start( "git", args );
	result	=	proc->waitForFinished();

	//
	if( result == true )
	{
		output	=	proc->readAll();
		remain_msg	=	"";

		while( output.length() > 0 )
		{
			str		=	splite_git_output( output );

			if( str.length() > 4 )
				parse_short_status( list, str );
		}
	}

	return	list;
}*/




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
QColor		GitStatus::get_status_color( const QString& status )
{
	QColor		color	=	QColor();

	if( status == GIT_STATUS_TRACKED )
		color	=	QColor(Qt::darkGreen);
	else if( status == GIT_STATUS_ADDED )
		color	=	QColor(Qt::blue);
	else if( status == GIT_STATUS_MODIFY )
		color	=	QColor(Qt::red);
	else
		color	=	QColor(Qt::black);

	return	color;
}




/*******************************************************************
	get_file_status
********************************************************************/
QString		GitStatus::get_file_status( QString path, QString filename )
{
	//qDebug() << filename;
	if( filename == QString("..") )
		return	QString("");

	QProcess		*proc	=	new QProcess();
	QStringList		args;

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
			status	=	GIT_STATUS_TRACKED;

		else if( output.contains("modified:") == true )
			status	=	GIT_STATUS_MODIFY;

		else if( output.contains("new file:") == true )
			status	=	GIT_STATUS_ADDED;

		else if( output.contains("Untracked files:") == true )
			status	=	GIT_STATUS_UNTRACKED;

		else if( output.contains("deleted:") == true )
			status	=	GIT_STATUS_DELETED;

		else
			ERRLOG("error. filename = %s, output = %s", qPrintable(filename), qPrintable(output) )
		
	}
	else
		status	=	QString("");

	delete	proc;
	return	status;
}
