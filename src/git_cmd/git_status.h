#ifndef GIT_STATUS_H
#define GIT_STATUS_H

#include <QString>
#include "git_base.h"
#include "../def.h"


#define GIT_STATUS_TRACKED		"tracked"
#define GIT_STATUS_MODIFY		"modified"
#define GIT_STATUS_ADDED		"added"
#define GIT_STATUS_UNTRACKED	"untracked"
#define GIT_STATUS_DELETED		"deleted"






/*******************************************************************
	GitStatus
********************************************************************/
class   GitStatus : public GitBase
{
public:
    GitStatus( QObject *parent = 0 );
    ~GitStatus();
    
	QString			get_file_status( QString path, QString filename );
	QColor			get_file_color( QString path, QString filename );
	QStringList		get_delete_files( QString path );

	FileInfoList	get_all_status( QString path );
	void			parse_short_status( FileInfoList &list, const QString &str );

	static QColor	get_status_color( const QString& status );
    
private:
};





#endif
