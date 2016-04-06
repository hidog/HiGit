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
QString    GitCommit::commit( QString root_path, QString msg, GitParameter param )
{
    QProcess        *proc   =   new QProcess(this);
    QStringList     args;
    QByteArray      output, err_msg;
    
    proc->setWorkingDirectory( root_path );
    
    args << "commit";
    
    if( param.find(GIT_COMMIT_AUTHOR) != param.end() )
        args << QString("--author=\"") + param[GIT_COMMIT_AUTHOR] + QString("\"");
    
    if( param.find(GIT_COMMIT_DATE) != param.end() )
        args << QString("--date=\"") + param[GIT_COMMIT_DATE] + QString("\"");
    
    args << ( QString("--message=\"") + msg + QString("\"") );
    
    proc->start( "git", args );
  
    if( proc->waitForFinished() == true )
        output = proc->readAll();
    else
        ERRLOG("commit read fail")
    
    // get if error.
    err_msg     =   proc->readAllStandardError();
    if( err_msg.length() > 0 )
        output  =   err_msg;
        
    delete  proc;
    
    // add color
    if( output.contains("fatal:") )
        set_color( output, GIT_FONT_RED );
    else
        set_color( output, GIT_FONT_BLUE );
    
    return  QString(output);
}