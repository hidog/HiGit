#ifndef GIT_COMMIT_H
#define GIT_COMMIT_H


#include "git_base.h"

#define GIT_COMMIT_DATE     QString("date")
#define GIT_COMMIT_AUTHOR   QString("author")


/*******************************************************************
	GitCommit
********************************************************************/
class   GitCommit : public GitBase
{
    Q_OBJECT
    
public:
    GitCommit( QObject *parent = NULL );
    ~GitCommit();
    
    QString    commit( QString root_path, QString msg, GitParameter param );
    
private:
    
    
};



#endif