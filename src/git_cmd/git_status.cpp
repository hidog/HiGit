#include "git_status.h"

#include <QProcess>
#include "../def.h"
#include <QDebug>
#include <QColor>
#include <QTextCodec>
#include "../tools.h"

#ifndef Q_MOC_RUN
#	include <boost/bind.hpp>
#endif


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
STATUS_TYPE		GitStatus::parse_short_status( const QByteArray &status )
{
	char		X,	Y;	// naming see above URL.

	X		=	status[0];
	Y		=	status[1];

	// decide X
	switch( X )
	{
		case 'A':
			return	STATUS_ADDED;

		case 'M' :	
			return	STATUS_MODIFIED;

		case 'D':
			return	STATUS_DELETED;
	}

	// decide Y
	switch( Y )
	{
		case 'M' :	
			return	STATUS_MODIFIED;

		case 'A' :
			return	STATUS_ADDED;

		case '?' :
			return	STATUS_UNTRACKED;

		case 'D':
			return	STATUS_DELETED;

		default:
			// there are something status not define, see above URL.
			ERRLOG("status fail.")
	}

	return	STATUS_DEFAULT;
}



/*******************************************************************
	git_status_list
********************************************************************/
template<typename DataType>
QList<DataType>		GitStatus::git_status_list( QString path, int filter, HANDLE_FUNC_t handle_func )
{
	QList<DataType>	list;
	DataType		data;

	QProcess		*proc	=	new QProcess();
	QStringList		args;
	QByteArray		output;
	QByteArray		str;
	QByteArray		status,	filename;
	STATUS_TYPE		type;

	proc->setWorkingDirectory( path );
	args << "status" << "-s";

	proc->start( "git", args );

	//
	if( true == proc->waitForFinished() )
		output	=	proc->readAll();
	else
	{
		ERRLOG("git status fail.")
		return	list;
	}

	// parse output.
	while( output.length() > 0 )
	{
		str		=	splite_git_output( output );

		if( str.length() > 4 )
		{
			status		=	str.mid( 0, 2 );
			type		=	parse_short_status( status );

			if( type & filter )
			{
				filename	=	str.mid( 3, -1 );
				handle_func( list, type, path, filename );
			}
		}
	}

	delete	proc;
    return	list;
}



/*******************************************************************
	get_untracked_files_func
********************************************************************/
void	GitStatus::get_untracked_files_func( QList<QFileInfo> &list, STATUS_TYPE type, const QString path, QByteArray filename )
{
	QFileInfo	info;

	info	=	QFileInfo( path + QString("/") + filename );
	if( info.isFile() )
		list.push_back(info);
}

/*******************************************************************
	get_untracked_files
********************************************************************/
QFileInfoList	GitStatus::get_untracked_files( QString path )
{
	boost::function< void( QList<QFileInfo>&, STATUS_TYPE, QString, QByteArray& ) >		handle_func;
	handle_func	=	boost::bind( &GitStatus::get_untracked_files_func, this, _1, _2, _3, _4 );

    return	git_status_list<QFileInfo>( path, STATUS_UNTRACKED, handle_func );
}


/*******************************************************************
	get_modify_files_func
********************************************************************/
void	GitStatus::get_modify_files_func( QList<FileStatus> &list, STATUS_TYPE type, const QString path, QByteArray filename )
{
	FileStatus	sts;

	// translate type.
	switch( type )
	{
		case STATUS_TRACKED:
			sts.status	=	GIT_STATUS_TRACKED;
			break;
		case STATUS_MODIFIED:
			sts.status	=	GIT_STATUS_MODIFY;
			break;
		case STATUS_ADDED:
			sts.status	=	GIT_STATUS_ADDED;
			break;
		case STATUS_UNTRACKED:
			sts.status	=	GIT_STATUS_UNTRACKED;
			break;
		case STATUS_DELETED:
			sts.status	=	GIT_STATUS_DELETED;
			break;
		default:
			ERRLOG("undefine type.")
	}

	//
	sts.name	=	filename;
	sts.color	=	get_status_color(type);

	//
	list.push_back(sts);
}




/*******************************************************************
	get_modify_list
********************************************************************/
FileStatusList	GitStatus::get_modify_files( QString path )
{
	boost::function< void( QList<FileStatus>&, STATUS_TYPE, QString, QByteArray& ) >	handle_func;
	handle_func	=	boost::bind( &GitStatus::get_modify_files_func, this, _1, _2, _3, _4 );

    return	git_status_list<FileStatus>( path, STATUS_ALL, handle_func );
}



/*******************************************************************
	get_file_color
********************************************************************/
QColor		GitStatus::get_file_color( QString path, QString filename )
{
	QString		status	=	get_file_status( path, filename );
	QColor		color	=	get_status_color( status );

	return	color;
}


/*******************************************************************
	get_file_color
********************************************************************/
QColor		GitStatus::get_status_color( const QString& status )
{
	QColor		color	=	QColor();

	if( status == GIT_STATUS_TRACKED )
		color	=	get_status_color(STATUS_TRACKED);
	else if( status == GIT_STATUS_MODIFY )
		color	=	get_status_color(STATUS_MODIFIED);
	else if( status == GIT_STATUS_ADDED )
		color	=	get_status_color(STATUS_ADDED);
	else if( status == GIT_STATUS_UNTRACKED )
		color	=	get_status_color(STATUS_UNTRACKED);
	else if( status == GIT_STATUS_DELETED )
		color	=	get_status_color(STATUS_DELETED);
	//else
		//ERRLOG("undefined status = %s", status.toStdString().c_str() )

	return	color;
}

/*******************************************************************
	get_file_color
********************************************************************/
QColor	GitStatus::get_status_color( const STATUS_TYPE status )
{
	QColor	color	=	QColor();

	switch( status )
	{
		//case STATUS_DEFAULT:
			//color	=	QColor(Qt::black);
			//break;
		case STATUS_TRACKED:
			color	=	QColor(Qt::darkGreen);
			break;
		case STATUS_MODIFIED:
			color	=	QColor(Qt::red);
			break;
		case STATUS_ADDED:
			color	=	QColor(Qt::blue);
			break;
		case STATUS_UNTRACKED:
			color	=	QColor(Qt::black);
			break;
		case STATUS_DELETED:
			color	=	QColor(Qt::darkRed);
			break;
		default:
			ERRLOG("status not defined.")
	}

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
