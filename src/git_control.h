#include <QObject>
#include <QProcess>
#include <QString>

#include <string>


#define	GIT_BUF_SIZE	1000


using namespace std;




/*******************************************************************
	GitControl
********************************************************************/
class	GitControl : public QObject
{
	Q_OBJECT

public:
	GitControl();
	~GitControl();
	
	bool		check_git_exist();
	string		get_version();

	void		init( QString path );
	void		clone( QString src, QString dest );

signals:
	void		output_signal(QByteArray);
	void		output_signal(QList<QByteArray>);
	void		progress_signal(int);

protected slots:
	void		error_slot( QProcess::ProcessError err );

	void		clone_output_err_slot();
	void		clone_output_std_slot();
	void		clone_output_slot();


	void		clone_finish_slot(int,QProcess::ExitStatus);
	void		clone_start_slot();
	void		clone_error_slot(QProcess::ProcessError);


private:
	void		set_connect();

	void		clone_parse_end( QByteArray& data, QByteArray& msg );
	void		clone_parse_num( int index, QByteArray& output, QByteArray& data, QByteArray& msg );

	QList<QByteArray>	output_list;	// 為了讓畫面能呈現百分比更新的樣子,用list來parse字串.
	QByteArray			last_msg;		// parse字串時使用,紀錄上一次的訊息. 主要是希望view端能夠有只有數字在更新的效果.

	char	msg_buf[GIT_BUF_SIZE];		// git output message buffer.
	int		last_index;					// last read index.

};
