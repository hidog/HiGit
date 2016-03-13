#include "tools.h"




/*******************************************************************
	qstring_contain_full_width
********************************************************************/
bool	qstring_contain_full_width( const QString &str )
{
	foreach( QChar ch, str )
	{
		if( ch.unicode() > 0x7F )
			return	true;
	}

	return	false;

	/*
	for( QString::iterator itr = info.name.begin(); itr != info.name.end(); ++itr )
	{
		if( itr->unicode() > 0x7F )
		{
			info.status			=	git_status.get_file_status( info.path, info.name );
			info.font_color		=	git_status.get_status_color( info.status );
			break;
		}
	}*/
}
