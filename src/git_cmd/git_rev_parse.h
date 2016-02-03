#ifndef GIT_REV_PARSE_H
#define GIT_REV_PARSE_H

#include <QString>



/*******************************************************************
	GitStatus
********************************************************************/
class   GitRevParse
{
public:
    GitRevParse();
    ~GitRevParse();
    
	QString		get_root_path( QString path );
    
private:
};





#endif
