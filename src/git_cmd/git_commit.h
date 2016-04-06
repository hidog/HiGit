#ifndef GIT_COMMIT_H
#define GIT_COMMIT_H


#include "git_base.h"


/*******************************************************************
	GitCommit
********************************************************************/
class   GitCommit : public GitBase
{
    Q_OBJECT
    
public:
    GitCommit( QObject *parent = NULL );
    ~GitCommit();
    
    void    commit( QString root_path, QString msg );
    
private:
    
    
};



#endif