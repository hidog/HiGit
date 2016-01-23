#ifndef GIT_CLONE_H
#define GIT_CLONE_H

#include "git_command.h"


/*******************************************************************
	GitClone
 ********************************************************************/
class   GitClone : public GitCommand
{
    Q_OBJECT
    
public:
    GitClone( QWidget *parent = 0 );
    virtual ~GitClone();
    
    
private:
    
};





#endif