#ifndef GIT_STATUS_H
#define GIT_STATUS_H

#include <QString>
#include "git_command.h"
#include "../def.h"


/*******************************************************************
	GitStatus
********************************************************************/
class   GitStatus : public GitCommand
{
public:
    GitStatus( QObject *parent = 0 );
    ~GitStatus();
    
	QString			get_file_status( QString path, QString filename );
	QStringList		get_delete_files( QString path );

	void	exec( GitParameter param ) { ERRLOG("no use.") }

    
private:
};





#endif
