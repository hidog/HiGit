#include "git_control.h"

#include <iostream>
#include <cassert>
#include <sstream>
#include <string>

#include <QDebug>
#include <QMessageBox>


using  namespace std;



/*******************************************************************
	GitControl
********************************************************************/
GitControl::GitControl()
	:	QObject()
{
	set_connect();
}


/*******************************************************************
	GitControl
********************************************************************/
GitControl::~GitControl()
{
}


/*******************************************************************
	GitControl
********************************************************************/
void	GitControl::set_connect()
{
	//connect(	proc,	SIGNAL(readyRead()),					this,	SLOT(on_read())								);
	//connect(	proc,	SIGNAL(error(QProcess::ProcessError)),	this,	SLOT(error_slots(QProcess::ProcessError))	);
}




/*******************************************************************
	check_git_exist
********************************************************************/
bool	GitControl::check_git_exist()
{
	QProcess		*proc	=	new QProcess(this);
	QStringList		args;

	bool	result;

	args << "--version";
	proc->start( "git", args );

	if( proc->waitForFinished() )
		result	=	true;
	else
		result	=	false;

	delete	proc;
	return	result;
}




/*******************************************************************
	init
********************************************************************/
void	GitControl::init( QString path )
{
	QProcess		*proc	=	new QProcess(this);
	QStringList		args;

	args << "init";
	args << path;

	proc->start( "git", args );

	if( proc->waitForFinished() )
		QMessageBox::about( NULL, "init", "init success." );
	else
		QMessageBox::critical( NULL, "init", "init fail." );		

	delete	proc;
}





/*******************************************************************
	clone
********************************************************************/
void	GitControl::clone( QString src, QString dest )
{
    gproc	=	new QProcess(this);
	QStringList		args;

	connect(	gproc,	SIGNAL(readyRead()),	this,	SLOT(on_read())		);
    
	args << "clone";
	args << src;
	args << dest;
    
	gproc->start( "git", args );

	if( gproc->waitForFinished() )
		QMessageBox::about( NULL, "clone", "init success." );
	else
		QMessageBox::critical( NULL, "clone", "init fail." );		

	delete	gproc;
}




/*******************************************************************
	get_version
********************************************************************/
string		GitControl::get_version()
{
	QProcess		*proc	=	new QProcess(this);
	QStringList		args;
	string			version;
	stringstream	ss;

	args << "--version";
	proc->start( "git", args );

	if( proc->waitForFinished() )
		ss << proc->readAll().data();
	else
		ss << "git not exists.";

	return	version;
}


/*******************************************************************
	error_slots
********************************************************************/
void	GitControl::error_slot( QProcess::ProcessError err )
{
	cout << "QProcess get err = " << err << endl;
}




/*******************************************************************
	on_read
********************************************************************/
void    GitControl::on_read()
{
    cout << qPrintable(gproc->readAll());
}
