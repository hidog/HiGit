#ifndef GIT_CLONE_H
#define GIT_CLONE_H

#include "git_command.h"


#define	GIT_CLONE_SOURCE		QString("source")
#define GIT_CLONE_DESTINATION	QString("destination")
#define GIT_CLONE_RECURSIVE		QString("recursive")


/*******************************************************************
	GitClone
 ********************************************************************/
class   GitClone : public GitCommand
{
    Q_OBJECT
    
public:
    GitClone( QWidget *parent = 0 );
    virtual ~GitClone();
    
	void	exec( GitParameter param );


public slots:
	void		clone_output_err_slot();
	void		clone_output_std_slot();
	void		clone_output_slot();

	void		clone_finish_slot(int,QProcess::ExitStatus);
	void		clone_start_slot();
	void		clone_error_slot(QProcess::ProcessError);
    
private:
    
	QString		type, host,	content;		
	QString		username, password;
	int			port;

	void		clone_parse_end( QByteArray& data, QByteArray& msg );
	void		clone_parse_num( int index, QByteArray& output, QByteArray& data, QByteArray& msg );

};





#endif