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
	void	push_output_err_slot();
	void	push_output_std_slot();
	void	push_output_slot();

	void	push_finish_slot( int, QProcess::ExitStatus );
	void	push_start_slot();
	void	push_error_slot( QProcess::ProcessError );
	
	//void	abort_slot();

private:

};

#endif