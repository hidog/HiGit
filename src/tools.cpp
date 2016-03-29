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





/*******************************************************************
	get_extension
********************************************************************/
QString		get_extension( QString filename )
{
	int		index	=	filename.lastIndexOf('.');
	if( index < 0 )
		return	QString("");
	else
		return	filename.mid( index+1 );
}





/*******************************************************************
	get_filesize_str
********************************************************************/
QString		get_filesize_str( qint64 size )
{
	int			count	=	0;
	double		d		=	size;
	QString		unit	=	QString();
	QString		str;

	//
	while( d > 1024 )
	{
		d	/=	1024;
		count++;
	}

	//
	switch(count)
	{
		case 0:
			unit	=	"Bytes";
			break;
		case 1:
			unit	=	"KB";
			break;
		case 2:
			unit	=	"MB";
			break;
		case 3:
			unit	=	"GB";
			break;
	}

	//
	if( count == 0 )
		str		=	str.sprintf( "%lld %s", size, unit.toStdString().c_str() );
	else
		str		=	str.sprintf( "%.1lf %s", d, unit.toStdString().c_str() );

	return	str;
}

