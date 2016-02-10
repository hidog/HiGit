#ifndef GIT_LOG_H
#define GIT_LOG_H

#include "git_command.h"

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
class GitLog : public GitCommand
{
	Q_OBJECT

public:
	GitLog( QObject *parent = 0 );
	~GitLog();

	void	exec( GitParameter param );

	LogDataList		get_log_list( QString path );

	QString			get_commit( QByteArray str );
	QString			get_author( QByteArray str );
	QString			get_date( QByteArray str );
	QString			get_title( QByteArray str );


private:



};


#endif