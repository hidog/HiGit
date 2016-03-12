#ifndef GIT_COMMAND_H
#define GIT_COMMAND_H

#include <QObject>
#include <QProcess>
#include <QString>
#include "git_base.h"

#include <string>

#ifndef Q_MOC_RUN
#	include <boost/container/map.hpp>
#	include <boost/function.hpp>
#endif


#define	GIT_BUF_SIZE	1000


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ typedef ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef boost::container::map<QString,QString>	GitParameter;





/*******************************************************************
	GitCommand
********************************************************************/
class	GitCommand : public GitBase
{
	Q_OBJECT

public:
	GitCommand( QObject *parent = 0 );
	virtual ~GitCommand();
	
	virtual	void	exec( GitParameter param )	=	0;

	void	parse_host( QString str, QString &type, QString &host, QString &username, QString &password, int &port, QString &content );

	boost::function<void(int)>					set_progress_func;
	boost::function<void(QList<QByteArray>)>	set_ui_dynamic_output_func;
	boost::function<void()>						input_user_passwd_func;


signals:

	void		finished_signal();


protected:
	void			splite_progress( QByteArray data, QByteArray &msg, int &num );
	void			set_progess( int num );
	void			refresh_dynamic_output( QByteArray data, QByteArray msg );


private:




};

#endif
