#include "git_control.h"

#include <iostream>
#include <cassert>
#include <sstream>
#include <string>

#include <QDebug>
#include <QMessageBox>

#include "def.h"



using  namespace std;



/*******************************************************************
	GitControl
********************************************************************/
GitControl::GitControl( QWidget *parent )
	:	QObject(parent), last_index(0), ppp(parent)
{
	memset( msg_buf, 0, GIT_BUF_SIZE );	
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
	�Ϊ������ˬd
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
	QProcess		*proc	=	new QProcess(this);
	QStringList		args;
	
	args << "clone";
	args << "-v";
	args << "--progress";

	if( get_recursive_state_func() == true )
		args << "--recursive";

	//args << src;
	// ����https��. �ݭn�󥿳o�䪺code
	// http://blog.coolsea.net/archives/240
	// linux���U���G���ssh�Ӱ������ݬO�_�ݭn�b���K�X  �M��windows�����u��.
	args << "https://hidog:yas12129@bitbucket.org/hidog/lpd";
	args << dest;
	//args << "www.google.com";

	// init data.
	last_index	=	0;
	output_list.clear();
	memset( msg_buf, 0, GIT_BUF_SIZE );
	
	connect(	proc,	SIGNAL(readyReadStandardError()),				this,	SLOT(clone_output_err_slot())						);
	connect(	proc,	SIGNAL(readyReadStandardOutput()),				this,	SLOT(clone_output_std_slot())						);
	connect(	proc,	SIGNAL(readyRead()),							this,	SLOT(clone_output_slot())							);
	connect(	proc,	SIGNAL(finished(int,QProcess::ExitStatus)),		this,	SLOT(clone_finish_slot(int,QProcess::ExitStatus))	);
	connect(	proc,	SIGNAL(started()),								this,	SLOT(clone_start_slot())							);
	connect(	proc,	SIGNAL(error(QProcess::ProcessError)),			this,	SLOT(clone_error_slot(QProcess::ProcessError))		);

	/*
		note: git clone will create thread, so need set process channel for get output in other thread.
	*/
	proc->setProcessChannelMode( QProcess::SeparateChannels );
	proc->start( "git", args, QProcess::ReadWrite );


	switch(proc->state())
	{
		case QProcess::NotRunning:
			PRINT_ENUM(QProcess::NotRunning);
			break;
		case QProcess::Starting:
			PRINT_ENUM(QProcess::Starting);
			break;
		case QProcess::Running:
			PRINT_ENUM(QProcess::Running);
			break;
		default:
			assert(0);
	}
}




/*******************************************************************
	clone_error_slot
********************************************************************/
void	GitControl::clone_error_slot( QProcess::ProcessError err )
{
	QByteArray	result;

	switch(err)
	{
		case QProcess::FailedToStart:
			PRINT_ENUM(QProcess::FailedToStart);
			break;
		case QProcess::Crashed:
			PRINT_ENUM(QProcess::Crashed);
			break;
		case QProcess::Timedout:
			PRINT_ENUM(QProcess::Timedout);
			break;
		case QProcess::WriteError:
			PRINT_ENUM(QProcess::WriteError);
			break;
		case QProcess::ReadError:
			PRINT_ENUM(QProcess::ReadError);
			break;
		case QProcess::UnknownError:
			PRINT_ENUM(QProcess::UnknownError);
			break;
		default:
			assert(0);
	}
}



/*******************************************************************
	clone_finish_slot
********************************************************************/
void	GitControl::clone_finish_slot( int exit_code, QProcess::ExitStatus exit_status )
{
	// delete proc
	QProcess	*proc	=	(QProcess*)sender();
	delete	proc;

	QByteArray	output;

	switch(exit_status)
	{
		case QProcess::NormalExit:
			PRINT_ENUM(QProcess::NormalExit);
			break;
		case QProcess::CrashExit:
			PRINT_ENUM(QProcess::CrashExit);
			break;
		default:
			assert(0);
	}
}



/*******************************************************************
	clone_start_slot
********************************************************************/
void	GitControl::clone_start_slot()
{
	qDebug() << "git clone start...";

	QProcess	*proc	=	(QProcess*)sender();
	//proc->write(".");

	QByteArray	output	=	proc->readAll();
	qDebug() << output ;

}


/*******************************************************************
	clone_output_slot
********************************************************************/
void	GitControl::clone_output_slot()
{
	QProcess	*proc	=	(QProcess*)sender();
	QByteArray	output	=	proc->readAll();	

	cout << qPrintable( output );
	//qDebug() << output << "end...";
	//emit( output_signal(output) );
}



/*******************************************************************
	set_color
********************************************************************/
void	GitControl::set_color( QByteArray& data, GIT_FONT_COLOR color )
{
	switch( color )
	{
		case GIT_FONT_RED:
			data.push_front("<font color=\"red\">");	
			data.push_back("</font>");
			break;
		case GIT_FONT_BLUE:
			data.push_front("<font color=\"blue\">");	
			data.push_back("</font>");
			break;
		default:
			assert(0);
	}
}



/*******************************************************************
	clone_parse_end
	�B�z�r�굲��,��s�e��������.
********************************************************************/
void		GitControl::clone_parse_end( QByteArray& data, QByteArray& msg )
{
	//qDebug() << data << "\n";
	if( data.indexOf(QString("fatal")) >= 0 )
		set_color( data, GIT_FONT_RED );
	else if( data.indexOf(QString("done")) >= 0 )
		set_color( data, GIT_FONT_BLUE );

	if( msg.size() == 0 )
		msg		=	data;	// �r��̭��S���ʤ���case.
	
	if( output_list.size() == 0 )
	{
		output_list.push_back(data);
		last_msg	=	msg;
	}
	else
	{
		// ��̫�@�Ӱ����,�ۦP���ܥ[�J.
		//qDebug() << last_msg << " " << msg << "\n";
		if( last_msg != msg )
		{
			output_list.push_back(data);
			last_msg	=	msg;
		}
		else
			output_list[output_list.size()-1]	=	data;	// ��s�̫�@�����.
	}

	//emit( output_signal(data) );		
	emit output_signal(output_list);
	data.clear();
}



/*******************************************************************
	clone_parse_num
	Ū���r��ʤ��񪺳���.
********************************************************************/
void	GitControl::clone_parse_num( int index, QByteArray& output, QByteArray& data, QByteArray& msg )
{
	QByteArray	num;		// �x�s�ʤ���� 
	QByteArray	tmp;

	int		start;
	int		i,	j;
	int		size;	
	int		percent;

	bool	ok;

	// parse number.
	data	+=	output[index];
	msg.clear();
	for( i = index-1; i >= 0; i-- )
	{
		if( output[i] >= '0' && output[i] <= '9' )
			num		+=	output[i];
		else if( output[i] == '.' )
			assert(0);		// ���T�w�ʤ��񦳨S���T�I��.
		else
			break;
	}
	// ���X msg ����
	start	=	0;
	for( j = i; j >= 0; j-- )
	{
		if( output[j] == '\n' || output[j] == '\r' )
		{
			start	=	j + 1;
			break;
		}
	}
	for( j = start; j <= i ; j++ )
		msg	+=	output[j];

	/*
		�o�䦳�@�Ӥw����bug
		�r�꦳�i���n�Q���}
		Ū���� "%   ....  remote:  99%" �o�˪��r��
		�y��parse���~.
		�٬O�o�ھ�\n�Ӥ��r��,�åB�O�s�e�@�����r��.
	*/
	qDebug() << output << " -----  ";

	// �����ťա@
	while(true)
	{
		if( msg.size() == 0 )
			break;
		if( msg[msg.size()-1] != ' ' )
			break;
		else
			msg.remove( msg.size()-1, 1 );
	}
	//qDebug() << msg;

	if( msg.size() == 0 )
		return;		// ��ܭ�nŪ��Ŧr�ꤧ����
	
	// �r�����
	size	=	num.size();
	for( i = 0; i < size; i++ )
		tmp	+=	num[size-i-1];
	// ���o�Ʀr
	percent		=	tmp.toInt( &ok, 10 );
	if( ok == true )
		emit( progress_signal(percent) );
}




/*******************************************************************
	clone_output_slot
********************************************************************/
void	GitControl::clone_output_err_slot()
{
	QProcess	*proc	=	(QProcess*)sender();
	QByteArray	output	=	proc->readAllStandardError();
	QByteArray	data;		// ���Τ�r�� ��msg���P,�o��|�]�t�᭱�ʤ��񪺳���.
	QByteArray	msg;		// ���]�r��O  Receive 5% (12/60)   msg�s�񪺬OReceive

	int		i;
    
    qDebug(output);

	// �B�z�r��.
	for( i = 0; i < output.size(); i++ )
	{
		if( output[i] == '\n' || output[i] == '\r' )
			clone_parse_end( data, msg );
		else if( output[i] == '%' )
			clone_parse_num( i, output, data, msg );
		else
			data	+=	output[i];
	}

	// �������O '\n', '\r', �N��S����s��e��.
	if( output[output.size()-1] != '\n' && output[output.size()-1] != '\r' )
		clone_parse_end( data, msg );
}


/*******************************************************************
	clone_output_slot
********************************************************************/
void	GitControl::clone_output_std_slot()
{
	QProcess	*proc	=	(QProcess*)sender();
	QByteArray	output	=	proc->readAllStandardOutput();

	cout << "std... " << qPrintable( output ) << " ... end output\n";

	//emit( output_signal(output) );
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




