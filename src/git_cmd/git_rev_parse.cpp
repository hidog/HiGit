#include "git_status.h"

#include <QProcess>

/*******************************************************************
	GitStatus
********************************************************************/
GitStatus::GitStatus()
{}


/*******************************************************************
	GitStatus
********************************************************************/
GitStatus::~GitStatus()
{}





/*******************************************************************
	open_folder
********************************************************************/
bool    GitStatus::open_folder( QString path )
{
    QProcess        *proc   =   new QProcess();
    QStringList     args;
    
    bool    result;
    
    args << "status";
    args << path;
    
    result  =   proc->waitForFinished();
    
    if( result == true )
    {
        QByteArray  output  =   proc->readAll();
        
        return  true;
    }
    else
        return  false;
}