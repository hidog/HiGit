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

	//cout << args.at(0).toStdString().c_str();
	//connect( proc, SIGNAL(error(QProcess::ProcessError)), this, SLOT(error_slot(QProcess::ProcessError)) );

	proc->start( "git", args );

	if( proc->waitForFinished() )
		QMessageBox::about( NULL, "init", "init success." );
	else
		QMessageBox::critical( NULL, "init", "init fail." );		

	delete	proc;
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

#if 1


/*
 void MainWindow::on_read()
{

QProcess *pProces = (QProcess *)sender();

QString result = pProces->readAll();

QMessageBox::warning(NULL, "", result.toLatin1() );

} */
#endif
