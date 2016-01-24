#ifndef GIT_COMMAND_H
#define GIT_COMMAND_H

#include <QObject>
#include <QProcess>
#include <QString>

#include <string>

#ifndef Q_MOC_RUN
#	include <boost/container/map.hpp>
#endif

#define	GIT_BUF_SIZE	1000


typedef boost::container::map<QString,QString>	GitParameter;


/*******************************************************************
	GitCommand
********************************************************************/
class	GitCommand : public QObject
{
	Q_OBJECT

public:
	GitCommand( QWidget *parent = 0 );
	virtual ~GitCommand();
	
	virtual	void	exec( GitParameter param )	=	0;

	void	parse_host( QString str, QString &type, QString &host, QString &username, QString &password, int &port, QString &content );

signals:



protected:
	void			splite_remain( QByteArray &output );

	QList<QByteArray>	output_list;	// use for update view message.s

	QByteArray		last_msg;		
	QByteArray		remain_msg;		

	char			msg_buf[GIT_BUF_SIZE];		// git output message buffer.
	int				last_index;					// last read index.


protected slots:


private:




};

#endif
