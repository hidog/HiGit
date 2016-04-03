#ifndef GIT_CONTROL_H
#define GIT_CONTROL_H

#include <QObject>
#include <QProcess>
#include <QString>
#include <string>

#include "def.h"

#ifndef Q_MOC_RUN
#	include <boost/function.hpp>
#endif

#include "git_cmd/git_command.h"



using namespace std;




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
enum	GIT_COMMAND_TYPE
{
	GIT_CMD_CLONE,
	GIT_CMD_STATUS,
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

	bool		init( QString path );
	void		clone( QString src, QString dest, bool is_local_path, QString username = "", QString password = "" );

	void		add( QString root_path, QList<QString> str_list );

	void		set_progress( int num );
	void		set_ui_dynamic_output( QList<QByteArray> output_list );
	void		input_user_passwd();

	void		abort_cmd();

	QString		get_proj_name( QString path );
	QString		check_exist_git_repository( QString path );

	boost::function<bool()>		get_recursive_state_func;
	boost::function<bool()>		get_depth_state_func;
	boost::function<int()>		get_depth_num_func;

protected slots:

	void		cmd_finished_slot();


signals:
	void		output_signal(QByteArray);
	void		output_signal(QList<QByteArray>);
	void		progress_signal(int);
	void		need_user_pw_signal();

	void		cmd_finished_signal();
	void		abort_signal();

protected slots:
	void		error_slot( QProcess::ProcessError err );


private:
	void		set_connect();





};

#endif
