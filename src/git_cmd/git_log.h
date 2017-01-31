#ifndef GIT_LOG_H
#define GIT_LOG_H

#include "git_base.h"
#include "git_log_graph.h"
#include <boost/container/list.hpp>



#define DEFAULT_LOG_SIZE "-100";


struct LogData
{
	QString		commit;
	QString		author;
	QString		date;
	QString		title;
};

typedef QList<LogData>		LogDataList;

class GitGraphLine;


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

	// git log --graph
	int			get_log_graph( QString path, GitLineList& line_list, QList<int> &node_list );
	void		add_node_and_init_line( QByteArray& str, QList<GitGraphLine>& line_list, int count );
	bool		handle_graph_data( QByteArray& str, QList<GitGraphLine>& line_list, QList<int>& node_list, int count );
	int			parse_hash_decorate( const QByteArray& str, QString &hash, QString &decorate, bool &is_node );
	void		graph_node_handler( QList<GitGraphLine> &line_list, QList<int>& node_list, int locate, QString &hash, QString &decorate, int count );
	void		graph_fork( QList<GitGraphLine>& line_list, int locate, bool is_node );
	int			search_next_left( int n, QByteArray& str );

private:
    QString		get_commit( QByteArray str );
    QString		get_author( QByteArray str );
    QString		get_date( QByteArray str );
    QString		get_title( QByteArray str );


};


#endif