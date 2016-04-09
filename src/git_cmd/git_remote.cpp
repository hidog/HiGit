#include "git_remote.h"

#include <QProcess>
#include <QDebug>
#include "../def.h"


/*******************************************************************
	GitRemote
********************************************************************/
GitRemote::GitRemote( QString _root_path, QObject *parent ) :
	root_path(_root_path),
	GitBase(parent)
{}




/*******************************************************************
	GitRemote
********************************************************************/
GitRemote::~GitRemote()
{}




/*******************************************************************
	parse_remote_name_url
********************************************************************/
RemoteInfo		GitRemote::parse_remote_name_url( QByteArray str )
{
	QRegExp		rexp("(\\S+)\\t");
	int			pos		=	0;
	RemoteInfo	remote;

	pos		=	rexp.indexIn( str, pos );
	if( pos != -1 )
	{
		remote.name		=	rexp.cap(1);
		//qDebug() << remote.name;
		pos		+=	rexp.matchedLength();
	}
	else
	{
		ERRLOG("parse remote name fail.")
		return	remote;
	}

	rexp	=	QRegExp("(\\S+)");
	pos		=	rexp.indexIn( str, pos );
	if( pos != -1 )
	{
		remote.url	=	rexp.cap(1);
		//qDebug() << remote.url;
	}
	else
	{
		ERRLOG("parse remote url fail.")
		return	remote;
	}

	return	remote;
}



/*******************************************************************
	get_remote_list
********************************************************************/
RemoteInfoList		GitRemote::get_remote_list()
{
	QProcess		*proc	=	new QProcess(this);
	QStringList		args;
	QByteArray		output,	str;
	RemoteInfoList	list;
	RemoteInfo		remote;

	bool	search_result;

	args << "remote" << "-v";
	proc->setWorkingDirectory(root_path);

	proc->start( "git", args );

	if( proc->waitForFinished() )
		output	=	proc->readAll();
	else
	{
		ERRLOG("git remote fail.")
		return	list;
	}

	//
	while( output.length() > 0 )
	{
		str		=	splite_git_output( output );
		//qDebug() << str;
		remote	=	parse_remote_name_url(str);

		if( remote.name.length() <= 0 )
			continue;

		// avoid repeat remote.
		search_result	=	false;
		foreach( RemoteInfo info, list )
		{
			if( info.name == remote.name )
			{
				search_result	=	true;
				break;
			}
		}

		if( search_result == false )
			list.push_back(remote);
	}

	delete	proc;
	return	list;
}



