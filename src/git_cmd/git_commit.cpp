#include "git_commit.h"

#include "../def.h"
#include <QDebug>


/*******************************************************************
	GitCommit
********************************************************************/
GitCommit::GitCommit( QObject *parent ) :
    GitBase(parent)
{}



/*******************************************************************
	GitCommit
 ********************************************************************/
GitCommit::~GitCommit()
{}



/*******************************************************************
	commit
 ********************************************************************/
void    GitCommit::commit( QString root_path, QString msg )
{
    QProcess        *proc   =   new QProcess(this);
    QStringList     args;
    
    proc->setWorkingDirectory( root_path );
    
    args << "commit";
    args << "-m=" << msg ;
    
    proc->start( "git", args, QIODevice::ReadWrite );
    
    /*if( proc->waitForBytesWritten() == true )
        proc->write( msg );
    else
        ERRLOG("commit write fail")*/
    
    if( proc->waitForFinished() == true )
    {
        QByteArray output = proc->readAll();
        qDebug() << output;
    }
    else
        ERRLOG("commit read fail")
        
    delete  proc;
        
    
}