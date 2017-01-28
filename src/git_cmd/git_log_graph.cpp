#include "git_log_graph.h"

#include <QDebug>




/*******************************************************************
	GitGraphLine
********************************************************************/
GitGraphLine::GitGraphLine( int _index, int _current )
	:	index(_index), current(_current), is_end_flag(false), last_operator(0), is_handle_round(true)
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
	print_data
********************************************************************/
void	GitGraphLine::print_data()
{
	qDebug() << "current = " << current << " , index = " << index << " , is_end = " << (is_end_flag?1:0) << " lop = " << last_operator;
	QList<GitGraphNode>::iterator	itr;
	for( itr = node_list.begin(); itr != node_list.end(); ++itr )
	{
		if( itr->is_node == true )
			qDebug() << "nc = " << itr->node_count << " hash = " << itr->hash_code;
	}
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
	get_handle_round
********************************************************************/
bool	GitGraphLine::get_handle_round()
{
	return	is_handle_round;
}



/*******************************************************************
	set_handle_round
********************************************************************/
void	GitGraphLine::set_handle_round( bool _flag )
{
	is_handle_round		=	_flag;
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
	mark_vertical
********************************************************************/
void	GitGraphLine::mark_vertical()
{
	last_operator	=	git_log::vertical;
}




/*******************************************************************
	left_move
********************************************************************/
void	GitGraphLine::left_move( int target )
{
	if( target < 0 )
		ERRLOG( "target < 0. target = %d", target )

	current			=	target;
	last_operator	=	git_log::left;
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
	handle_poitr
********************************************************************/
void	handle_poitr( int locate, GitLineList& list )
{
	QList<GitGraphLine>::iterator	itr;
	for( itr = list.begin(); itr != list.end(); ++itr )
	{
		if( itr->get_current() == locate )
			itr->set_handle_round( true );
	}
}


/*******************************************************************
	right_move
********************************************************************/
void	left_move( int locate, int target, GitLineList& list, bool force )
{
	if( locate < 0 )
		ERRLOG("locate < 0. locate = %d", locate)

	QList<GitGraphLine>::iterator	itr;
	for( itr = list.begin(); itr != list.end(); ++itr )
	{
		if( itr->is_end() == true )
			continue;
		else if( itr->get_handle_round() == true )
			continue;
		else if( itr->get_current() == locate ) 
		{
			if( force == true )
				itr->left_move( target );
			else if( itr->get_last_operator() == git_log::left )
				itr->left_move( target );
		}
	}
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
	mark_vertical
********************************************************************/
void	mark_vertical( int locate, GitLineList& list, bool force )
{
	GitLineList::iterator	itr;
	for( itr = list.begin(); itr != list.end(); ++itr )
	{
		if( itr->get_current() == locate )
		{
			if( force == true )
			{
				itr->mark_vertical();
				itr->set_handle_round( true );
			}
			else if( itr->get_last_operator() == git_log::vertical )
			{
				itr->mark_vertical();
				itr->set_handle_round( true );
			}
		}
	}
}





/*******************************************************************
	init_round
********************************************************************/
void	init_round( GitLineList& list )
{
	GitLineList::iterator	itr;
	for( itr = list.begin(); itr != list.end(); ++itr )
		itr->set_handle_round(false);		
}



/*******************************************************************
	print_list
********************************************************************/
void	print_list( GitLineList& list )
{
	//qDebug() << "list size = " << list.size();
	GitLineList::iterator	itr;
	for( itr = list.begin(); itr != list.end(); ++itr )
	{
		itr->print_data();
		qDebug() << "--------------------------------------------------";
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
	QList<GitGraphLine>::iterator	first_itr	=	list.end();
	for( itr = list.begin(); itr != list.end(); ++itr )
	{
		if( itr->get_current() == locate )
		{
			itr->set_last_as_node( hash, decorate );
			if( first_itr == list.end() )
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
				itr->set_last_operator( git_log::node );

				if( itr != first_itr )
					itr->mark_end();
			}
		}
	}
}


} // end namespace git_log