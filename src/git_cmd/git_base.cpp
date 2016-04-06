#include "git_base.h"
#include "../def.h"

#include <QDebug>


/*******************************************************************
	GitControl
********************************************************************/
GitBase::GitBase( QObject *parent )
	:	QObject(parent)
{
}


/*******************************************************************
	GitControl
********************************************************************/
GitBase::~GitBase()
{
}



/*******************************************************************
	get_proj_name
********************************************************************/
QString		GitBase::get_proj_name( QString path )
{
	if( *(path.end()-1) == '/' || *(path.end()-1) == '\\' )
		path.remove( path.size()-1, 1 );

	int		tmp1	=	path.lastIndexOf( '/' );
	int		tmp2	=	path.lastIndexOf( '\\' );

	int		index	=	tmp1 > tmp2 ? tmp1 : tmp2;

	if( index != -1 )
	{
		QRegExp		rexp( "(\\w+)" );
		QString		name;

		if( rexp.indexIn( path, index ) != -1 )
			name	=	rexp.cap(1);
		else
			name	=	QString("default_name");

		return	name;
	}
	else
		return	QString("default_name");
}



/*******************************************************************
	splite_remain
********************************************************************/
void	GitBase::splite_remain( QByteArray &output )
{
	QByteArray	tmp		=	remain_msg + output;
	int		i, index;

	// search lst '\r', '\n'
	index	=	tmp.length();
	for( i = tmp.length()-1; i >= 0; i-- )
	{
		if( tmp[i] == '\r' || tmp[i] == '\n' )
		{
			index	=	i;
			break;
		}
	}

	output		=	tmp.mid( 0, index );
	remain_msg	=	tmp.mid( index+1 );
}




/*******************************************************************
	need_password
********************************************************************/
bool	GitBase::need_password( QByteArray data )
{
	if( data.contains("/dev/tty") )
		return	true;
	else
		return	false;
}







/*******************************************************************
	set_color
********************************************************************/
void	GitBase::set_color( QByteArray& data, GIT_FONT_COLOR color )
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
    
    while( data.contains("\n") == true )
        data.replace( "\n", "<br>" );
}




/*******************************************************************
	splite_git_output
********************************************************************/
QByteArray		GitBase::splite_git_output( QByteArray &output )
{
	QByteArray	data	=	"";
	int		i, index;
	int		size	=	output.length();

	//
	for( i = 0; i < size; i++ )
	{
		if( output[i] == '\r' || output[i] == '\n' )
		{
			index	=	i;
			break;
		}
	}

	// first is end charector
	if( i == 0 )
		output.remove( 0, 1 );
	else
	{
		data	=	output.mid( 0, index );
		output.remove( 0, index + 1 );
	}	

#ifdef LINUX
	// needed!! otherwise, it will ignore by compiler optimize.
    qDebug() << data;
#endif

	return	data;
}
