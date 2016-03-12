#ifndef GIT_STATUS_H
#define GIT_STATUS_H

#include <QString>
#include "git_base.h"
#include "../def.h"


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
	QColor			get_status_color( QString status );
	QStringList		get_delete_files( QString path );


    
private:
};





#endif
