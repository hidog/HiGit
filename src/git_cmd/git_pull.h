#ifndef GIT_PULL_H
#define GIT_PULL_H

#include "git_command.h"




/*******************************************************************
	GitPull
********************************************************************/
class GitPull : public GitCommand
{
	Q_OBJECT

public:
	GitPull( QObject *parent = 0 );
	~GitPull();

	void	exec( GitParameter param );


private:

};





#endif