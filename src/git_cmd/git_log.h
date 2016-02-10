#ifndef GIT_LOG_H
#define GIT_LOG_H

#include "git_command.h"


/*******************************************************************
	GitLog
********************************************************************/
class GitLog : public GitCommand
{
	Q_OBJECT

public:
	GitLog( QObject *parent = 0 );
	~GitLog();

	void	exec( GitParameter param );


	void	test( QString path );


private:



};


#endif