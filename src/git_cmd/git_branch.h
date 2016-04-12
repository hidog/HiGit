#ifndef GIT_BRANCH_H
#define GIT_BRANCH_H

#include "git_base.h"







/*******************************************************************
	GitBranch
********************************************************************/
class	GitBranch : public GitBase
{
	Q_OBJECT
public:
	GitBranch( QString _root_path, QObject *parent = 0 );
	~GitBranch();

	QString			current_branch();
	QStringList		get_all_branch();
	QStringList		get_remote_branch( QString remote );

private:

	QString		root_path;

};



#endif