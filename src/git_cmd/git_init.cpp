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
	bool	proc_result;
    
    args << "init";
    args << path;

	proc->start( "git", args );    
    proc_result  =   proc->waitForFinished();
    
    if( proc_result == true )
    {
        QByteArray  output  =   proc->readAll();

		if( output.contains("Initialized empty Git repository") == true )
		{
			QMessageBox::about( NULL, "init", "init success" );		
			result	=	true;
		}
		else if( output.contains("Reinitialized existing Git repository") == true )
		{
			QMessageBox::about( NULL, "init", "re-init." );		
			result	=	true;
		}
		else
		{
			QMessageBox::about( NULL, "init", "error" );		
			result	=	false;
		}
    }
    else
	{
		QMessageBox::about( NULL, "init", "init fail" );		
		result	=	false;
	}

	delete	proc;
	return	result;
}