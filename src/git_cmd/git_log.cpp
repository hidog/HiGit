#include "git_log.h"	

#include <QDebug>
#include "../def.h"
#include <stdio.h>



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
	get_commit
********************************************************************/
QString		GitLog::get_commit( QByteArray str )
{
	str.replace( "commit ", "" );
	return	QString(str);	
}




/*******************************************************************
	get_author
********************************************************************/
QString		GitLog::get_author( QByteArray str )
{
	str.replace("Author: ", "");

	/*QRegExp		rexp("[^<]");
	QString		result("");

	if( rexp.indexIn( str, 0 ) != -1 )
		result	=	rexp.cap(1);
	else
		ERRLOG("format error.")*/

	char	buf[1000];
	sscanf( str.toStdString().c_str(), "%[^<]", buf );

	QString		result(buf);

	//qDebug() << result;

	return	result;
}




/*******************************************************************
	get_date
********************************************************************/
QString		GitLog::get_date( QByteArray str )
{
	str.replace("Date: ", "");
	return	QString(str);
}



/*******************************************************************
	get_title
********************************************************************/
QString		GitLog::get_title( QByteArray str )
{
	str.remove( 0, 4 );

	return	QString("%1\n").arg(QString(str));
}



/*******************************************************************
	get_log_list
********************************************************************/
LogDataList	GitLog::get_log_list( QString path )
{
	QProcess		*proc	=	new QProcess(this);
	QStringList		args;

	LogDataList		list;
	LogData			log_data;

	proc->setWorkingDirectory( path );

	args << "log";
	args << "-100";

	proc->start( "git", args );

	bool	res		=	proc->waitForFinished();

	QByteArray	output,	str, title;

	if( res )
	{
		log_data.commit.clear();
		title.clear();
		output	=	proc->readAll();
		//qDebug() << output;

		while( output.length() > 0 )
		{
			str		=	splite_git_output(output);
			//qDebug(str);
			if( str.contains("commit") )
			{
				if( log_data.commit.length() > 0 )
				{
					// add one commit.
					log_data.title	=	title;
					list.push_back(log_data );
					log_data.commit.clear();
					title.clear();
				}

				// new commit.
				log_data.commit		=	get_commit( str );
			}
			else if( str.contains("Author:") )
				log_data.author		=	get_author( str );
			else if( str.contains("Date:") )
				log_data.date		=	get_date( str );
			else if( str.mid(0,4) == QString("    ") )
				title	+=	get_title( str );

		}
	}

	/*boost::container::list<LogData>::iterator	itr;
	for( itr = list.begin(); itr != list.end(); ++itr )
		qDebug() << itr->title << "\n\n";*/	

	delete	proc;
	return	list;
}





/*******************************************************************
	get_file_list
	只列出修改的檔案清單 
********************************************************************/
QStringList		GitLog::get_file_list( QString path, QString commit )
{
	QProcess		*proc	=	new QProcess(this);
	QStringList		args;

	QStringList		list;

	proc->setWorkingDirectory( path );

	args << "show";
	args << commit;
	args << "--name-only";
	args << "--pretty=format:";

	proc->start( "git", args );

	bool	res		=	proc->waitForFinished();

	QByteArray	output, str;

	if( res )
	{
		output	=	proc->readAll();

		while( output.length() > 0 )
		{
			str		=	splite_git_output(output);
			//qDebug() << "str = " << str << " end";
			/*if( str.contains("commit ") == false	&& 
				str.contains("Author: ") == false	&&
				str.contains("Date: ") == false		&&
				str.contains("Merge: ") == false	&& 
				str.size() > 0 && str.mid(0,4) != QString("    ") )
			{
				list << QString(str);
			}*/
			if( str.size() > 0 )
				list << QString(str);
		}
	}

	/*QStringList::iterator	itr;
	for( itr = list.begin(); itr != list.end(); ++itr )
		qDebug() << *itr;*/

	delete	proc;
	return	list;
}
