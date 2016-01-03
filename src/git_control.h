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

	QList<QByteArray>	output_list;	// ���F���e����e�{�ʤ����s���ˤl,��list��parse�r��.
	QByteArray			last_msg;		// parse�r��ɨϥ�,�����W�@�����T��. �D�n�O�Ʊ�view�ݯ�����u���Ʀr�b��s���ĪG.

	char	msg_buf[GIT_BUF_SIZE];		// git output message buffer.
	int		last_index;					// last read index.

};
