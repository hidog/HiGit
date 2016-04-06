#ifndef GIT_BASE_H
#define GIT_BASE_H

#include <QObject>
#include <QProcess>
#include <QString>

#include <string>

#ifndef Q_MOC_RUN
#	include <boost/container/map.hpp>
#endif


#define	GIT_BUF_SIZE	1000


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ typedef ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef boost::container::map<QString,QString>	GitParameter;



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
enum	GIT_FONT_COLOR
{
	GIT_FONT_RED,
	GIT_FONT_BLUE,
};



/*******************************************************************
	GitBase
********************************************************************/
class	GitBase : public QObject
{
	Q_OBJECT

public:
	GitBase( QObject *parent = 0 );
	virtual ~GitBase();

	static QString	get_proj_name( QString path );

signals:

	void		finished_signal();

protected:
	void			splite_remain( QByteArray &output );
	QByteArray		splite_git_output( QByteArray &output );
	bool			need_password( QByteArray data );
	void			set_color( QByteArray& data, GIT_FONT_COLOR color );


	QList<QByteArray>	output_list;	// use for update view message.s

	QByteArray		last_msg;		
	QByteArray		remain_msg;		

private:




};

#endif
