#ifndef GIT_STATUS_H
#define GIT_STATUS_H

#include <QString>



/*******************************************************************
	GitStatus
********************************************************************/
class   GitStatus
{
public:
    GitStatus();
    ~GitStatus();
    
    bool    open_folder( QString path );
    
private:
};





#endif
