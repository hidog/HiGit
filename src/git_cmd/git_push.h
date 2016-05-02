#ifndef GIT_PUSH_H
#define GIT_PUSH_H

#include "git_command.h"

#define GIT_PUSH_ROOT_PATH	QString("root_path")



/*******************************************************************
	GitPush
********************************************************************/
class GitPush : public GitCommand
{
	Q_OBJECT

public:
	GitPush( QObject *parent = 0 );
	~GitPush();

	void	exec( GitParameter param );

public slots:
	/*void	pull_output_err_slot();
	void	pull_output_std_slot();
	void	pull_output_slot();

	void	pull_finish_slot( int, QProcess::ExitStatus );
	void	pull_start_slot();
	void	pull_error_slot( QProcess::ProcessError );
		
	void	abort_slot();*/

private:

};

#endif