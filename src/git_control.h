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
    void    on_read();

private:
	QProcess	*gproc;

	void	set_connect();

};
