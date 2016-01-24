#include "git_command.h"
#include "def.h"

#include <QDebug>


/*******************************************************************
	GitControl
********************************************************************/
GitCommand::GitCommand( QWidget *parent )
{
}


/*******************************************************************
	GitControl
********************************************************************/
GitCommand::~GitCommand()
{
}




/*******************************************************************
	splite_remain
********************************************************************/
void	GitCommand::splite_remain( QByteArray &output )
{

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
		ERRLOG("format error.")
	type		=	str.mid( 0, type_index );
	//qDebug() << type;

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
		//qDebug() << username;
		//qDebug() << password;
	}
	else
		up_index	=	type_index + 3;

	// parse host. contain port, etc
	host	=	str.mid( up_index );
	//qDebug() << host;

	// parse port and host
	port_index	=	str.indexOf( ":", up_index );
	if( port_index != -1 )
	{
		if( rexp.indexIn( str, port_index ) != -1 )
			port	=	rexp.cap(1).toInt();
		//qDebug() << port;
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
		//qDebug() << content;
	}
}
