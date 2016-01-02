#include <QObject>
#include <QProcess>
#include <QString>

#include <string>

using namespace std;

/*******************************************************************
	GitControl
********************************************************************/
class	GitControl : QObject
{
	Q_OBJECT

public:
	GitControl();
	~GitControl();
	
	bool		check_git_exist();
	string		get_version();

	void		init( QString path );
	void		clone( QString src, QString dest );

protected slots:
	void	error_slot( QProcess::ProcessError err );

	void	clone_output_slot();
	void	clone_finish_slot(int,QProcess::ExitStatus);
	void	clone_start_slot();
	void	clone_error_slot(QProcess::ProcessError);

private:
	void	set_connect();

};
