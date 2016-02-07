#ifndef GIT_COMMAND_H
#define GIT_COMMAND_H

#include <QObject>
#include <QProcess>
#include <QString>

#include <string>

#ifndef Q_MOC_RUN
#	include <boost/container/map.hpp>
#	include <boost/function.hpp>
#endif


#define	GIT_BUF_SIZE	1000


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ typedef ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef boost::container::map<QString,QString>	GitParameter;


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
enum	GIT_FONT_COLOR
{
	GIT_FONT_RED,
	GIT_FONT_BLUE,
};



/*******************************************************************
	GitCommand
********************************************************************/
class	GitCommand : public QObject
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

	static QString	get_proj_name( QString path );

signals:

	void		finished_signal();


protected:
	void			splite_remain( QByteArray &output );
	QByteArray		splite_git_output( QByteArray &output );
	bool			need_password( QByteArray data );
	void			splite_progress( QByteArray data, QByteArray &msg, int &num );
	void			set_progess( int num );
	void			refresh_dynamic_output( QByteArray data, QByteArray msg );
	void			set_color( QByteArray& data, GIT_FONT_COLOR color );


	QList<QByteArray>	output_list;	// use for update view message.s

	QByteArray		last_msg;		
	QByteArray		remain_msg;		

protected slots:

	virtual void	abort_slot();

private:




};

#endif
