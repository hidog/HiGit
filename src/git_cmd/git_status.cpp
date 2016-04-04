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
            //sts.status	=	GIT_STATUS_ADDED;
			return	STATUS_ADDED;
	}

	// decide Y
	switch( Y )
	{
		case 'M' :	
            //sts.status	=	GIT_STATUS_MODIFY;
			return	STATUS_MODIFIED;

		case 'A' :
            //sts.status	=	GIT_STATUS_ADDED;
			return	STATUS_ADDED;

		case '?' :
            //sts.status	=	GIT_STATUS_UNTRACKED;
			return	STATUS_UNTRACKED;

		case 'D':
			//sts.status	=	GIT_STATUS_DELETED;
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

			//filename	=	str.mid( 3, -1 );
			/*if( status == "??" )
			{
				info	=	QFileInfo( path + QString("/") + filename );
				if( info.isFile() )
					list.push_back( info );
			}*/
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

/*
	QProcess		*proc	=	new QProcess();
	QStringList		args;
	bool			result;
	QByteArray		output;
	QByteArray		str;
	QByteArray		status,	filename;
	QFileInfoList	list;
	QFileInfo		info;

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
			{
				status		=	str.mid( 0, 2 );
				filename	=	str.mid( 3, -1 );

				if( status == "??" )
				{
					info	=	QFileInfo( path + QString("/") + filename );
					if( info.isFile() )
						list.push_back( info );
				}
			}
		}
	}

    return	list;
*/
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
StatusList	GitStatus::get_modify_files( QString path )
{
	boost::function< void( QList<FileStatus>&, STATUS_TYPE, QString, QByteArray& ) >	handle_func;
	handle_func	=	boost::bind( &GitStatus::get_modify_files_func, this, _1, _2, _3, _4 );

	return	git_status_list<FileStatus>( path, STATUS_ALL, handle_func );

/*
	QProcess		*proc	=	new QProcess();
	QStringList		args;
	bool			result;
	QByteArray		output;
	QByteArray		str;
	QByteArray		status,	filename;
	QStatusVec		vec;
	FileStatus		fs;
	QFileInfo		info;

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
			{
				status		=	str.mid( 0, 2 );
				filename	=	str.mid( 3, -1 );

				info	=	QFileInfo( path + QString("/") + filename );
				if( info.isFile() )
				{
					fs	=	parse_short_status( status );

					fs.name			=	filename;
					fs.size			=	QString("%1").arg(info.size());
					fs.extension	=	get_extension( filename );

					vec.push_back( fs );
				}
			}
		}
	}

    return	vec;
*/
}



/*******************************************************************
	get_delete_files_func
********************************************************************/
/*
void	GitStatus::get_delete_files_func( QList<QString> &list, STATUS_TYPE type, const QString path, QByteArray filename )
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
//#if 0
					// 移除folder的sample code.
					if( index >= 0 )
						filename.remove( 0, index+1 );
//#endif

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
//#endif
}
*/



/*******************************************************************
	get_delete_files
********************************************************************/
/*
QStringList		GitStatus::get_delete_files( QString path )
{
	boost::function< void( QList<QString>&, STATUS_TYPE, QString, QByteArray& ) >	handle_func;
	handle_func	=	boost::bind( &GitStatus::get_delete_files_func, this, _1, _2, _3, _4 );

	return	git_status_list<QString>( path, STATUS_DELETED, handle_func );

#if 0
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
//#endif

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
//#endif
}
*/



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



/*******************************************************************
	parse_short_status
	file:///C:/Users/hidog/AppData/Local/Programs/Git/mingw64/share/doc/git-doc/git-status.html
	this function call for FileWidget.
********************************************************************/
/*
void	GitStatus::get_all_status_parser( QStatusVec &vec, const QByteArray &str )
{
	QString		name	=	str.mid( 3 );

	// don't needed handle sub directory files.
	// also do not handle chinese file.
	if( name.contains("/") || name.contains("\\") )
		return;

    FileStatus	sts	=	parse_short_status(str);

	// don't add delete file.
	if( sts.status == GIT_STATUS_DELETED )
		return;

	vec.push_back(sts);
}
*/


/*******************************************************************
	get_all_status
	it is no use now.
********************************************************************/
/*
QStatusVec  GitStatus::get_all_status( QString path )
{
	QProcess		*proc	=	new QProcess();
	QStringList		args;
	bool			result;
    QStatusVec      vec;
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
                get_all_status_parser( vec, str );
		}
	}

    return	vec;
}
*/