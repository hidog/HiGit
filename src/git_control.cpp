#include "git_control.h"
#include <iostream>
#include <cassert>

using  namespace std;



/*******************************************************************
	GitControl
********************************************************************/
GitControl::GitControl()
	:	proc(NULL)
{
	proc	=	new QProcess();
	if( proc == NULL )
		assert(0);

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
	GitControl
********************************************************************/
QString		GitControl::get_version()
{
	QStringList		args;
	args << "--version";

	proc->start( "git", args );

	return	QString("");
}


/*******************************************************************
	error_slots
********************************************************************/
void	GitControl::error_slots( QProcess::ProcessError err )
{
	cout << "QProcess get err = " << err << endl;
}

#if 0
void	MainWindow::on_error(QProcess::ProcessError err)
{
	printf("%d\n", err );
}



 void MainWindow::on_read()
{

QProcess *pProces = (QProcess *)sender();

QString result = pProces->readAll();

QMessageBox::warning(NULL, "", result.toLatin1() );

} 
#endif
