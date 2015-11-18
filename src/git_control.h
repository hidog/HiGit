#include <QObject>
#include <QProcess>


/*******************************************************************
	GitControl
********************************************************************/
class	GitControl : QObject
{
public:
	GitControl();
	~GitControl();
	
	QString		get_version();

protected slots:
	void	get_version_slots();
	void	error_slots( QProcess::ProcessError err );

private:
	QProcess	*proc;

	void	set_connect();

};
