#include "git_clone.h"
#include "def.h"

#include <QProcess>


/*******************************************************************
	GitClone
********************************************************************/
GitClone::GitClone( QWidget *parent )
    :   GitCommand(parent)
{}


/*******************************************************************
	GitClone
********************************************************************/
GitClone::~GitClone()
{}




/*******************************************************************
	exec
********************************************************************/
void	GitClone::exec( GitParameter param )
{
	QProcess		*proc	=	new QProcess(this);
	QStringList		args;
	QString			src		=	param[GIT_CLONE_SOURCE];
	QString			dest	=	param[GIT_CLONE_DESTINATION];
	
	args << "clone";
	args << "-v";
	args << "--progress";

	if( param.find(GIT_CLONE_RECURSIVE) != param.end() )
		args << "--recusive";

	// get username, password, project name.
	parse_host( src, type, host, username, password, port, content );

	args << src;
	args << dest;

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
		note: need use -v, otherwise no message.
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
void	GitClone::clone_error_slot( QProcess::ProcessError err )
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
void	GitClone::clone_finish_slot( int exit_code, QProcess::ExitStatus exit_status )
{
	// delete proc
	QProcess	*proc	=	(QProcess*)sender();
	delete		proc;

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
void	GitClone::clone_start_slot()
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
void	GitClone::clone_output_slot()
{
	QProcess	*proc	=	(QProcess*)sender();
	QByteArray	output	=	proc->readAll();	

	qDebug() << output;
	//qDebug() << output << "end...";
	//emit( output_signal(output) );
}




/*******************************************************************
	clone_parse_end
	�B�z�r�굲��,��s�e��������.
********************************************************************/
void	GitClone::clone_parse_end( QByteArray& data, QByteArray& msg )
{
	qDebug() << data << "\n";
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
		qDebug() << last_msg << " " << msg << "\n";
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
void	GitClone::clone_parse_num( int index, QByteArray& output, QByteArray& data, QByteArray& msg )
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
void	GitClone::clone_output_err_slot()
{
	QProcess	*proc	=	(QProcess*)sender();
	//QByteArray	output	=	remain_msg + proc->readAllStandardError();
	QByteArray	output	=	proc->readAllStandardError();

	QByteArray	data;		// ��l���,�]�t�ʤ���
	QByteArray	msg;		// ���]�r��O  Receive 5% (12/60)   msg�s�񪺬OReceive

	int		i;

	// get string this term. keep some string in remain_str if needed.
	splite_remain( output );

	// �B�z�r��.
	for( i = 0; i < output.size(); i++ )
	{
		if( output[i] == '\n' || output[i] == '\r' )
		{
			if( data.indexOf(QString("bash: /dev/tty: No such device or address")) >= 0 )	// �ݭn�b�K
			{
				proc->kill();
				emit( need_user_pw_signal() );
			}
			clone_parse_end( data, msg );
		}
		else if( output[i] == '%' )
			clone_parse_num( i, output, data, msg );
		else
			data	+=	output[i];
	}

	// �������O '\n', '\r', �N��S����s��e��.
	if( output[output.size()-1] != '\n' && output[output.size()-1] != '\r' )
	{
		//clone_parse_end( data, msg );
		remain_msg	=	data;
		qDebug(remain_msg);
	}
	else
		remain_msg.clear();
}


/*******************************************************************
	clone_output_slot
********************************************************************/
void	GitClone::clone_output_std_slot()
{
	QProcess	*proc	=	(QProcess*)sender();
	QByteArray	output	=	proc->readAllStandardOutput();

	cout << "std... " << qPrintable( output ) << " ... end output\n";

	//emit( output_signal(output) );
}

