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
	int		locate;
	int		index;
	GitGraphFork( int _locate, int _index ) : locate(_locate), index(_index) {}
};

struct	GitGraphNode 
{
	bool	is_node;
	bool	is_merged;
	bool	is_fork;
	char	hash_code[30];
	std::string				decorate;		// tag, branch name, ... etc
	QList<GitGraphFork>		fork_list;

	GitGraphNode() : is_node(false), is_merged(false), is_fork(false)
	{
		// note: if modify hash_code size, also need modify the below code
		memset( hash_code, 0, 30 * sizeof(char) );
		fork_list.clear();
	}
};



/*******************************************************************
	GitGraphLine
	一個物件以一條線為單位,紀錄該條branch的資訊.
********************************************************************/
class GitGraphLine
{
public:
	GitGraphLine( int _index, int _current );
	~GitGraphLine();

	GET_MACRO( int, current );
	GET_MACRO( int, index );

	void	add_node();
	void	set_last_as_node( const QString &hash, const QString &decorate );
	void	set_last_as_merged();

	void	fork_line( int locate, int index );

	void	right_move();

private:
	QList<GitGraphNode>		node_list;

	int		current;	// current locate. 
	int		index;
};



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ typedef ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef QList<GitGraphLine>		GitLineList;


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace git_log{

GitGraphLine*	find_line( int locate, GitLineList& list );

void 	set_line_as_node( int locate, GitLineList& list, const QString &hash, const QString &decorate );
void	add_node( GitLineList& list );
void	right_move( int locate, GitLineList& list );


} // end namespace git_log




#endif