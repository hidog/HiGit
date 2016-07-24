#include "git_log_graph.h"






/*******************************************************************
	GitGraphLine
********************************************************************/
GitGraphLine::GitGraphLine( int _index, int _current )
	:	index(_index), current(_current), is_end_flag(false), last_operator(0)
{
	node_list.clear();
}



/*******************************************************************
	set_last_operator
********************************************************************/
void	GitGraphLine::set_last_operator( char lo )
{
	last_operator	=	lo;
}



/*******************************************************************
	GitGraphLine
********************************************************************/
GitGraphLine::~GitGraphLine()
{}



/*******************************************************************
	mark_end
********************************************************************/
void	GitGraphLine::mark_end()
{
	is_end_flag	=	true;
}




/*******************************************************************
	is_end
********************************************************************/
bool	GitGraphLine::is_end()
{
	return	is_end_flag;
}



/*******************************************************************
	set_last_as_node
********************************************************************/
void	GitGraphLine::set_last_as_node( const QString &hash, const QString &decorate )
{
	GitGraphNode	&node	=	node_list.last();
	node.is_node	=	true;
	node.decorate	=	decorate.toStdString();

	strcpy( node.hash_code, hash.toStdString().c_str() );
}


/*******************************************************************
	set_last_as_merged
********************************************************************/
void	GitGraphLine::set_last_as_merged()
{
	if( node_list.last().is_node == false )
		ERRLOG("it is merged but not node.")

	node_list.last().is_merged	=	true;
}


/*******************************************************************
	add_node
********************************************************************/
void	GitGraphLine::add_node( int count )
{
	node_list.push_back( GitGraphNode(count) );
}



/*******************************************************************
	right_move
********************************************************************/
void	GitGraphLine::right_move()
{
	current	+=	2;
	last_operator	=	git_log::right;
}




/*******************************************************************
	fork_line
********************************************************************/
void	GitGraphLine::fork_line( int locate, int index )
{
	int		i,	n;

	// find last node.
	n	=	-1;
	for( i = node_list.size()-1; i >= 0; i-- )
	{
		if( node_list[i].is_node == true )
		{
			n	=	i;
			break;
		}
	}

	if( n < 0 )
		ERRLOG("node not found.")

	// old data need increase locate.
	QList<GitGraphFork>::iterator	itr;
	QList<GitGraphFork>&	list	=	node_list[n].fork_list;
	for( itr = list.begin(); itr != list.end(); ++itr )
		itr->locate	+=	2;

	// add fork line.
	node_list[n].fork_list.push_back( GitGraphFork(locate,index) );

	// record operator.
	last_operator	=	git_log::vertical;
}




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace git_log {



	
/*******************************************************************
	find_line
********************************************************************/
GitGraphLine*	find_line( int locate, GitLineList& list )
{
	QList<GitGraphLine>::iterator	itr;
	for( itr = list.begin(); itr != list.end(); ++itr )
	{
		if( itr->get_current() == locate )
			return	&(*itr);
	}

	// not found.
	return	NULL;
}



/*******************************************************************
	right_move
********************************************************************/
void	right_move( int locate, GitLineList& list )
{
	if( locate < 0 )
		ERRLOG("locate < 0. locate = %d", locate)

	QList<GitGraphLine>::iterator	itr;
	for( itr = list.begin(); itr != list.end(); ++itr )
	{
		if( itr->get_current() == locate )
			itr->right_move();
	}
}



/*******************************************************************
	add_node
********************************************************************/
void	add_node( GitLineList& list, int count )
{
	QList<GitGraphLine>::iterator	itr;
	for( itr = list.begin(); itr != list.end(); ++itr )
	{
		if( itr->is_end() == false )
			itr->add_node( count );
	}
}




/*******************************************************************
	set_line_as_node
	note: multi-line can be a same node at the same locate.
********************************************************************/
void 	set_line_as_node( int locate, GitLineList& list, const QString &hash, const QString &decorate )
{
	int		count	=	0;

	QList<GitGraphLine>::iterator	itr;
	QList<GitGraphLine>::iterator	first_itr;
	for( itr = list.begin(); itr != list.end(); ++itr )
	{
		if( itr->get_current() == locate )
		{
			itr->set_last_as_node( hash, decorate );
			first_itr	=	itr;
			count++;
		}
	}

	// it is a merged node.
	if( count > 1 )
	{
		for( itr = first_itr; itr != list.end(); ++itr )
		{
			if( itr->get_current() == locate )
			{
				itr->set_last_as_merged();
				if( itr != first_itr )
					itr->mark_end();
			}
		}
	}
}


} // end namespace git_log
