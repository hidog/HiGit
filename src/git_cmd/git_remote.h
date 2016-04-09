#ifndef GIT_REMOTE_H
#define GIT_REMOTE_H

#include "git_base.h"



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
struct RemoteInfo
{
	QString		name;
	QString		url;
};
typedef	QList<RemoteInfo>	RemoteInfoList;


/*******************************************************************
	GitRemote
********************************************************************/
class	GitRemote : public GitBase
{
	Q_OBJECT
public:
	GitRemote( QString _root_path, QObject *parent = 0 );
	~GitRemote();

	RemoteInfoList		get_remote_list();
	RemoteInfo			parse_remote_name_url( QByteArray str );

private:

	const QString	root_path;


};


#endif