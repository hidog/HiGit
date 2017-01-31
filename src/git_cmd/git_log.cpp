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
	return: the size without decorate.
********************************************************************/
int		GitLog::parse_hash_decorate( const QByteArray& str, QString &hash, QString &decorate, bool &is_node )
{
	QRegExp		reg;

	int		i,	j,	pos;
	int		n;	// record the end of graph.
	int		size	=	str.length();

	hash.clear();
	decorate.clear();
	n		=	size;		// if @ not found, n is size.
	is_node	=	false;

	//
	for( i = 0; i < size; i++ )
	{
		if( str[i] == git_log::node )
			is_node	=	true;
		else if( str[i] == '@' )
		{
			n		=	i;
			reg		=	QRegExp("(\\w+)");
			pos		=	reg.indexIn( str, i+1 );
			if( pos != -1 )
				hash	=	reg.cap(1);
			else
				ERRLOG("hash format error")
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
			i	=	j + 1;
		}
	}

	return	n;
}



/*******************************************************************
	add_node
********************************************************************/
void	GitLog::add_node_and_init_line( QByteArray& str, QList<GitGraphLine>& line_list, int count )
{
	if( line_list.isEmpty() == true )
	{
		if( str[0] != '*' )
			ERRLOG("data format error.")

		// init line. 
		line_list.push_back( GitGraphLine( 0, 0 ) );
	}
	git_log::add_node( line_list, count );
}
	



/*******************************************************************
	graph_node_handler
********************************************************************/
void	GitLog::graph_node_handler( QList<GitGraphLine>& line_list, QList<int>& node_list, int locate, QString &hash, QString &decorate, int count )
{
	//qDebug() << hash << " " << decorate << endl;

	GitGraphLine	*line_ptr;
	int		index;
	int		node_index;

	if( hash.size() == 0 )
		ERRLOG("it is node without hash.");

	line_ptr	=	git_log::find_line( locate, line_list );
	if( line_ptr == NULL )
	{
		// not found, create new line. it will call when argument has -all
		index	=	line_list.size();
		line_list.push_back( GitGraphLine( index, locate ) );
		line_list.last().add_node( count );
		line_list.last().set_last_operator( git_log::node );
	}			
	else
		line_ptr->set_last_operator( git_log::node );

	//
	node_index	=	git_log::set_line_as_node( locate, line_list, hash, decorate );
	node_list.push_back(node_index);
}




/*******************************************************************
	graph_fork
********************************************************************/
void	GitLog::graph_fork( QList<GitGraphLine>& line_list, int locate, bool is_node )
{
	GitGraphLine	*line_ptr;
	int		index;
	int		new_line_locate		=	locate + 2;

	// add line
	index	=	line_list.size();
	line_list.push_back( GitGraphLine( index, new_line_locate ) );
	line_list.last().set_last_operator( git_log::right );

	if( is_node == true )
		ERRLOG("graph format error.")
		//line_list.last().add_node( count );

	//
	line_ptr	=	git_log::find_line( locate, line_list );
	if( line_ptr == NULL )
		ERRLOG("fork but not found")
	else
		line_ptr->fork_line( new_line_locate, index );

}




/*******************************************************************
	search_next_left
********************************************************************/
int		GitLog::search_next_left( int n, QByteArray& str )
{
	int		i;
	int		size	=	str.size();

	for( i = n; i < size; i += 2 )
	{
		if( str[i] == git_log::left )
			return	i;
	}

	ERRLOG("could not found left word.")
}


/*******************************************************************
	handle_graph_data
********************************************************************/
bool	GitLog::handle_graph_data( QByteArray& str, QList<GitGraphLine>& line_list, QList<int>& node_list, int count )
{
	QString		hash, decorate;

	int		i,	j;
	int		index,	n;
	bool	is_node		=	false;

	//qDebug(str);

	// parse hash and decorate.
	n	=	parse_hash_decorate( str, hash, decorate, is_node );

	//  add node. it will init line_list if line_list is empty.
	if( is_node == true )
		add_node_and_init_line( str, line_list, count );

	// parse line.
	git_log::init_round( line_list );
	for( i = n-1; i >= 0; i-- )
	{
		if( str[i] == git_log::node )
			graph_node_handler( line_list, node_list, i, hash, decorate, count );
		else if( str[i] == git_log::right  && str[i+1] == git_log::vertical )
			ERRLOG("graph format not support. %s", str.toStdString().c_str() )
		else if( str[i] == ' ' )
			continue;	// do nothing.
		else if( str[i] == git_log::right )
		{
			if( str[i-1] == git_log::vertical )
			{
				graph_fork( line_list, i-1, is_node );
				i--;
			}
			else
				git_log::right_move( i-1, line_list );
		}
		else if( str[i] == git_log::horizon )
		{
			// search the true branch (left). skip all horizon.
			j	=	search_next_left( i, str );
			git_log::left_move( j-1, i-1, line_list, "_" );
		}
		else if( str[i] == git_log::left )
		{
			if( str[i-1] == git_log::vertical )
			{
				git_log::mark_vertical( i-1, line_list, "|/" );		// note: 留意這邊會不會因為改變了last operator的狀態,造成bug.
				git_log::left_move( i+1, i-1, line_list, "|/" );
				i--;
			}
			else
				git_log::left_move( i+1, i-1, line_list, " /" );
		}
		else if( str[i] == git_log::vertical )
		{
			if( i > 0 ) 
			{
				if( str[i-1] == git_log::left )		//  /|
				{
					git_log::mark_vertical( i, line_list, "/|" );
					git_log::left_move( i, i-2, line_list, "/|" );
				}
				else
					git_log::mark_vertical( i, line_list, "|" );
			}
			else
				git_log::mark_vertical( i, line_list, "|" );
		}
		else
			ERRLOG("unknown pattern. str = %s ", qPrintable(str) )
	}

	//git_log::print_list( line_list );
	//qDebug() << "\n ~~~~~~~~~~~~~~~~~~~ end ~~~~~~~~~~~~~~~~~~~~\n";

	return	is_node;
}



/*******************************************************************
	get_log_graph
********************************************************************/
int		GitLog::get_log_graph( QString path, GitLineList &line_list, QList<int> &node_list )
{
	QProcess		*proc	=	new QProcess(this);
	QStringList		args;

	bool	is_node;

	proc->setWorkingDirectory( path );

	args << "log";
	args << DEFAULT_LOG_SIZE; 
	args << "--pretty= @%h %d";
	args << "--graph";

	proc->start( "git", args );

	bool	res		=	proc->waitForFinished();
	int		count	=	0;

	QByteArray		output,	str;
	//GitLineList		line_list;
	line_list.clear();

	if( res == false )
	{
		ERRLOG("git process fail.")
		delete	proc;
		return	0;
	}

	// parse data.
	output	=	proc->readAll();
	//qDebug() << qPrintable(output);

	while( output.length() > 0 )
	{
		str		=	splite_git_output(output) + ' ';
		is_node	=	handle_graph_data( str, line_list, node_list, count );
		if( is_node == true )
			count++;
	}
	
	//qDebug() << "end " << line_list.size();
	delete	proc;
	return	count;
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
	args << DEFAULT_LOG_SIZE;

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
