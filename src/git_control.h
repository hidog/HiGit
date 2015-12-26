#include <QObject>
#include <QProcess>

#include <string>

using namespace std;

/*******************************************************************
	GitControl
********************************************************************/
class	GitControl : QObject
{
public:
	GitControl();
	~GitControl();
	
	bool		check_git_exist();
	string		get_version();

protected slots:
	void	get_version_slots();
	void	error_slots( QProcess::ProcessError err );

private:
	//QProcess	*proc;

	void	set_connect();

};
