#include "git_command.h"
#include "../def.h"

#include <QDebug>


/*******************************************************************
	GitControl
********************************************************************/
GitCommand::GitCommand( QObject *parent )
	:	GitBase(parent)
{
}


/*******************************************************************
	GitControl
********************************************************************/
GitCommand::~GitCommand()
{
}






/*******************************************************************
	splite_progress
********************************************************************/
void	GitCommand::splite_progress( QByteArray data, QByteArray &msg, int &num )
{
	QRegExp		rexp("(\\d+%)");
	int			index;

	// remove multi space
	while( data.contains("  ") )
		data.replace( "  ", " " );

	// get num
	num		=	-1;
	msg		=	data;
	index	=	rexp.indexIn( data );
	if( index != -1 )
	{
		msg		=	data.mid( 0, index );
		num		=	rexp.cap( 1 ).remove('%').toInt();
	}
}






/*******************************************************************
	set_progess
********************************************************************/
void	GitCommand::set_progess( int num )
{
	if( set_progress_func )
		set_progress_func(num);
	else
		ERRLOG("no bind");
}

	
/*******************************************************************
	refresh_dynamic_output
********************************************************************/
void	GitCommand::refresh_dynamic_output( QByteArray data, QByteArray msg )
{
	// set color.
	if( data.indexOf(QString("fatal")) >= 0 )
		set_color( data, GIT_FONT_RED );
	else if( data.indexOf(QString("done")) >= 0 )
		set_color( data, GIT_FONT_BLUE );
	
	if( output_list.size() == 0 )
	{
		output_list.push_back(data);
		last_msg	=	msg;
	}
	else
	{
		// 跟最後一個做比較,相同的話加入.
		if( last_msg != msg )
		{
			output_list.push_back(data);
			last_msg	=	msg;
		}
		else
			output_list.last()	=	data;
	}

	set_ui_dynamic_output_func(output_list);
}






/*******************************************************************
	parse_host
	type://username:password@host:port
********************************************************************/
void	GitCommand::parse_host( QString str, QString &type, QString &host, QString &username, QString &password, int &port, QString &content )
{
	int			type_index, up_index, pass_index, port_index, content_index;
	QString		user_pass;
	QRegExp		rexp( "(\\d+)" );

	// default value. 
	username	=	"";
	password	=	"";
	port		=	-1;		// port < 0 means no given port.
	content		=	"";

	// parse type.  https, http, git://
	type_index	=	str.indexOf("://");
	if( type_index == -1 )
	{
		ERRLOG("format error.")
		return;
	}
	type		=	str.mid( 0, type_index );

	// pasrse username, password.
	up_index	=	str.indexOf( "@", type_index );
	if( up_index != -1 )
	{
		user_pass	=	str.mid( type_index + 3, up_index-type_index - 3 );
		pass_index	=	user_pass.indexOf(":");
		if( pass_index != -1 )
		{
			username	=	user_pass.mid( 0, pass_index );
			password	=	user_pass.mid( pass_index + 1 );
		}
		else
			username	=	user_pass;		
	}
	else
		up_index	=	type_index + 3;

	// parse host. contain port, etc
	host	=	str.mid( up_index );

	// parse port and host
	port_index	=	str.indexOf( ":", up_index );
	if( port_index != -1 )
	{
		if( rexp.indexIn( str, port_index ) != -1 )
			port	=	rexp.cap(1).toInt();
	}
	else
		port_index	=	up_index;

	// parse content. use for default project name.
	content_index	=	str.lastIndexOf( '/' );
	if( content_index != -1 )
	{
		rexp.setPattern( "(\\w+)" );
		if( rexp.indexIn( str, content_index ) != -1 )
			content		=	rexp.cap(1);
	}
}
