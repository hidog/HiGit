#ifndef GIT_LOG_H
#define GIT_LOG_H

#include "git_base.h"

#include <boost/container/list.hpp>


struct LogData
{
	QString		commit;
	QString		author;
	QString		date;
	QString		title;
};

typedef QList<LogData>		LogDataList;




/*******************************************************************
	GitLog
********************************************************************/
class GitLog : public GitBase
{
	Q_OBJECT

public:
	GitLog( QObject *parent = 0 );
	~GitLog();


	LogDataList		get_log_list( QString path );
	QStringList		get_file_list( QString path, QString commit );
    
    QString         last_author( QString path );
	void			get_log_graph( QString path );

private:
    QString			get_commit( QByteArray str );
    QString			get_author( QByteArray str );
    QString			get_date( QByteArray str );
    QString			get_title( QByteArray str );

};


#endif