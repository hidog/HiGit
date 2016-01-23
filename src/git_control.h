#ifndef GIT_CONTROL_H
#define GIT_CONTROL_H

#include <QObject>
#include <QProcess>
#include <QString>

#include <string>

#ifndef Q_MOC_RUN
#	include <boost/function.hpp>
#endif

#define	GIT_BUF_SIZE	1000


using namespace std;


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
enum	GIT_FONT_COLOR
{
	GIT_FONT_RED,
	GIT_FONT_BLUE,
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
	
	bool		check_git_exist();
	string		get_version();

	void		init( QString path );
	void		clone( QString src, QString dest );
	void		clone( QString src, QString dest, QString username, QString password );

	boost::function<bool()>		get_recursive_state_func;


signals:
	void		output_signal(QByteArray);
	void		output_signal(QList<QByteArray>);
	void		progress_signal(int);
	void		need_user_pw_signal();

protected slots:
	void		error_slot( QProcess::ProcessError err );

	void		clone_output_err_slot();
	void		clone_output_std_slot();
	void		clone_output_slot();


	void		clone_finish_slot(int,QProcess::ExitStatus);
	void		clone_start_slot();
	void		clone_error_slot(QProcess::ProcessError);


private:
	QWidget		*ppp;

	void		set_connect();

	void		set_color( QByteArray& data, GIT_FONT_COLOR color );
	void		clone_parse_end( QByteArray& data, QByteArray& msg );
	void		clone_parse_num( int index, QByteArray& output, QByteArray& data, QByteArray& msg );

	QList<QByteArray>	output_list;	// 為了讓畫面能呈現百分比更新的樣子,用list來parse字串.
	QByteArray			last_msg;		// parse字串時使用,紀錄上一次的訊息. 主要是希望view端能夠有只有數字在更新的效果.
	QByteArray			remain_msg;		// 被切斷 未parse的訊息

	char	msg_buf[GIT_BUF_SIZE];		// git output message buffer.
	int		last_index;					// last read index.

};

#endif
