#include "git_init.h"

#include <QProcess>
#include <QMessageBox>

/*******************************************************************
	GitStatus
********************************************************************/
GitInit::GitInit()
{}


/*******************************************************************
	GitStatus
********************************************************************/
GitInit::~GitInit()
{}





/*******************************************************************
	exec
********************************************************************/
bool    GitInit::exec( QString path )
{
    QProcess        *proc   =   new QProcess();
    QStringList     args;
    
    bool    result;
    
    args << "init";
    args << path;
    
    result  =   proc->waitForFinished();
    
    if( result == true )
    {
        QByteArray  output  =   proc->readAll();

		if( output.contains("Initialized empty Git repository") == true )
		{
			QMessageBox::about( NULL, "init", "init success" );		
			return	true;
		}
		else if( output.contains("Reinitialized existing Git repository") == true )
		{
			QMessageBox::about( NULL, "init", "re-init." );		
			return	true;
		}
		else
		{
			QMessageBox::about( NULL, "init", "error" );		
			return	false;
		}
    }
    else
	{
		QMessageBox::about( NULL, "init", "init fail" );		
		return	false;
	}
}