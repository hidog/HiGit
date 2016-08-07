#ifndef GIT_LOG_GRAPH_H
#define GIT_LOG_GRAPH_H

#include <QList>
#include "git_base.h"
#include "../def.h"



namespace git_log{
	const char node		=	'*';
	const char vertical	=	'|';
	const char right	=	'\\';
	const char left		=	'/';
	const char horizon	=	'_';
} // end namespace git_log


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
struct	GitGraphFork
{
	int		locate;		// locate where are forked. ����fork from����m
	int		index;		// locate who are forked from. ����fork from����H
	GitGraphFork( int _locate, int _index ) : locate(_locate), index(_index) {}
};

struct	GitGraphNode 
{
	bool	is_node;
	bool	is_merged;
	bool	is_fork;
	char	hash_code[30];
	int		node_count;						// recode counting of node.

	std::string				decorate;		// tag, branch name, ... etc
	QList<GitGraphFork>		fork_list;

	GitGraphNode( int _count ) 
		: is_node(false), is_merged(false), is_fork(false), node_count(_count)
	{
		// note: if modify hash_code size, also need modify the below code
		memset( hash_code, 0, 30 * sizeof(char) );
		fork_list.clear();
	}
};



/*******************************************************************
	GitGraphLine
	�@�Ӫ���H�@���u�����,�����ӱ�branch����T.
********************************************************************/
class GitGraphLine
{
public:
	GitGraphLine( int _index, int _current );
	~GitGraphLine();

	GET_MACRO( int, current )
	GET_MACRO( int, index )
	GET_MACRO( char, last_operator )

	void	add_node( int count );
	void	set_last_as_node( const QString &hash, const QString &decorate );
	void	set_last_as_merged();

	void	fork_line( int locate, int index );

	void	mark_end();
	bool	is_end();
	void	right_move();
	void	left_move( int target );

	void	set_last_operator( char lo );

private:
	QList<GitGraphNode>		node_list;

	int		current;				// current locate. 
	int		index;
	bool	is_end_flag;			// if a line is end, it means it has been merged.
	char	last_operator;

};



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ typedef ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef QList<GitGraphLine>		GitLineList;


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace git_log{

GitGraphLine*	find_line( int locate, GitLineList& list );

void 	set_line_as_node( int locate, GitLineList& list, const QString &hash, const QString &decorate );
void	add_node( GitLineList& list, int count );
void	right_move( int locate, GitLineList& list );
void	left_move( int locate, int target, GitLineList& list );
void	print_list( GitLineList& list );


} // end namespace git_log




#endif