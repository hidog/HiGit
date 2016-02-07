#ifndef GIT_CLONE_H
#define GIT_CLONE_H

#include "git_command.h"
#include "../def.h"

#define	GIT_CLONE_SOURCE		QString("source")
#define GIT_CLONE_DESTINATION	QString("destination")
#define GIT_CLONE_RECURSIVE		QString("recursive")
#define GIT_CLONE_USERNAME		QString("username")
#define GIT_CLONE_PASSWORD		QString("password")
#define GIT_CLONE_DEPTH			QString("depth")


/*******************************************************************
	GitClone
 ********************************************************************/
class   GitClone : public GitCommand
{
    Q_OBJECT
    
public:
    GitClone( QObject *parent = 0 );
    virtual ~GitClone();
    
	void	exec( GitParameter param );

signals:
	void		abort_signal();
	void		update_proj_button_signal( DbProj );


public slots:
	void		clone_output_err_slot();
	void		clone_output_std_slot();
	void		clone_output_slot();

	void		clone_finish_slot(int,QProcess::ExitStatus);
	void		clone_start_slot();
	void		clone_error_slot(QProcess::ProcessError);

	void		abort_slot();
    
private:
    
	QString		type, host,	content;		
	QString		username, password;
	int			port;

	QString		path,	name;		// local path and proj name.

};





#endif