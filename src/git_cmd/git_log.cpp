#include "git_log.h"	

#include <QDebug>
#include <QList>

#include "../def.h"
#include "git_log_graph.h"

#include <stdio.h>



/*******************************************************************
	GitLog
********************************************************************/
GitLog::GitLog( QObject *parent ) :
	GitBase(parent)
{}



/*******************************************************************
	GitLog
********************************************************************/
GitLog::~GitLog()
{}




/*******************************************************************
	parse_hash_decorate
********************************************************************/
int		GitLog::parse_hash_decorate( const QByteArray& str, QString &hash, QString &decorate)
{
	QRegExp		reg;

	int		i,	j,	pos;
	int		n;	// record the end of graph.
	int		size	=	str.length();

	hash.clear();
	decorate.clear();

	n	=	-1;

	//
	for( i = 0; i < size; i++ )
	{
		if( str[i] == '@' )
		{
			n		=	i;
			reg		=	QRegExp("(\\w+)");
			pos		=	reg.indexIn( str, i+1 );
			if( pos != -1 )
				hash	=	reg.cap(1);
			else
				ERRLOG("hash format error");
			//qDebug() << hash;
			i	=	pos + reg.matchedLength();
		}
		else if( str[i] == '(' )
		{
			decorate.clear();
			for( j = i+1; j < str.length(); j++ )
			{
				if( str[j] == ')' )
					break;
				else if( str[j] != ' ' )
					decorate	+=	str[j];
			}
			//qDebug() << branch;
			i	=	j + 1;
		}
	}

	return	n;
}

	


/*******************************************************************
	handle_graph_data
********************************************************************/
void	GitLog::handle_graph_data( QByteArray& str, QList<GitGraphLine>& line_list )
{
	QString		hash, decorate;

	int		i,	j;
	int		index,	n;
	bool	is_node;

	GitGraphLine	*line_ptr;

	qDebug(str);

	// parse hash and decorate.
	n	=	parse_hash_decorate( str, hash, decorate );

	// parse line.
	is_node		=	false;
	for( i = 0; i < str.length(); i++ )
	{
		//qDebug() << str[i] << " ";
		if( str[i] == git_log::node )
		{
			if( hash.size() == 0 )
				ERRLOG("it is node without hash.");

			is_node		=	true;
			line_ptr	=	git_log::find_line( i, line_list );
			if( line_ptr == NULL )
			{
				// not found, create new line.
				index	=	line_list.size();
				line_list.push_back( GitGraphLine( index, i ) );
			}

			git_log::add_node( line_list );			
			git_log::set_line_as_node( i, line_list, hash, decorate );
		}
		else if( (str[i] == git_log::left_move && str[i+1] == git_log::vertical) ||
			     (str[i] == git_log::vertical  && str[i+1] == git_log::right_move) )
		{
			ERRLOG("graph format not support. %s", str.toStdString() );
		}
		else if( str[i] == git_log::vertical && str[i+1] == git_log::right_move )
		{
			// fork
			index	=	line_list.size();
			line_list.push_back( GitGraphLine( index, i+2 ) );
			if( is_node == true )
				line_list.last().add_node();

			line_ptr	=	git_log::find_line( i, line_list );
			if( line_ptr == NULL )
				ERRLOG("fork but not found")
			else
				line_ptr->fork_line( i, index );
		}
		else if( str[i] == '/' )
		{}

	}
}



/*******************************************************************
	get_log_graph
********************************************************************/
void	GitLog::get_log_graph( QString path )
{
	QProcess		*proc	=	new QProcess(this);
	QStringList		args;

	LogDataList		list;
	LogData			log_data;

	proc->setWorkingDirectory( path );

	args << "log";
	args << "--pretty= @%h %d";
	args << "--graph";

	proc->start( "git", args );

	bool	res		=	proc->waitForFinished();

	QByteArray		output,	str;
	GitLineList		line_list;
	line_list.clear();

	if( res )
	{
		output	=	proc->readAll();
		qDebug() << qPrintable(output);

		while( output.length() > 0 )
		{
			str		=	splite_git_output(output) + ' ';
			handle_graph_data( str, line_list );
		}

	}

	delete	proc;
}





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
			if( str.mid(0,6) == QString("commit") )
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

	//LogDataList::iterator	itr;
	//for( itr = list.begin(); itr != list.end(); ++itr )
		//qDebug() << itr->title << "\n\n";

	delete	proc;
	return	list;
}




/*******************************************************************
	last_author
********************************************************************/
QString     GitLog::last_author( QString path )
{
    QProcess        *proc   =   new QProcess(this);
    QStringList     args;
    QByteArray      output;
    
    args << "log" << "-1" << QString("--pretty=format:%aN <%aE>");
    
    proc->start( "git", args );
    
    if( proc->waitForFinished() == true )
        output  =   proc->readAll();
    else
        ERRLOG("get author fail")
        
    //qDebug() << output;
        
    delete  proc;
    return  QString(output);
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
