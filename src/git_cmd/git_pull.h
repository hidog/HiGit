#ifndef GIT_PULL_H
#define GIT_PULL_H

#include "git_command.h"


#define GIT_PULL_ROOT_PATH	QString("root_path")



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

public slots:
	void	pull_output_err_slot();
	void	pull_output_std_slot();
	void	pull_output_slot();

	void	pull_finish_slot( int, QProcess::ExitStatus );
	void	pull_start_slot();
	void	pull_error_slot( QProcess::ProcessError );
		
	void	abort_slot();

private:

};





#endif