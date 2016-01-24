#ifndef GIT_CONTROL_H
#define GIT_CONTROL_H

#include <QObject>
#include <QProcess>
#include <QString>

#include <string>

#ifndef Q_MOC_RUN
#	include <boost/function.hpp>
#endif

#include "git_command.h"



using namespace std;


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
enum	GIT_FONT_COLOR
{
	GIT_FONT_RED,
	GIT_FONT_BLUE,
};

enum	GIT_COMMAND_TYPE
{
	GIT_CMD_CLONE,
};




/*******************************************************************
	GitControl
********************************************************************/
class	GitControl : public QObject
{
	Q_OBJECT

public:
	GitControl( QWidget *parent = 0 );
	~GitControl();
	
	void		exec_command( GIT_COMMAND_TYPE cmd_type, GitParameter param );

	bool		check_git_exist();
	string		get_version();

	void		init( QString path );
	void		clone( QString src, QString dest );
	//void		clone( QString src, QString dest, QString username, QString password );

	boost::function<bool()>		get_recursive_state_func;


signals:
	void		output_signal(QByteArray);
	void		output_signal(QList<QByteArray>);
	void		progress_signal(int);
	void		need_user_pw_signal();

protected slots:
	void		error_slot( QProcess::ProcessError err );




private:
	void		set_connect();

	void		set_color( QByteArray& data, GIT_FONT_COLOR color );




};

#endif
