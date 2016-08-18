#include "log_model.h"

#include <QDebug>
#include "../src/git_cmd/git_log.h"
#include "../src/def.h"


/*******************************************************************
	LogModel
********************************************************************/
LogModel::LogModel( QObject *parent ) :
	QAbstractTableModel( parent )
{
	head_list << "title" << "author" << "date";
}




/*******************************************************************
	FileWidget
********************************************************************/
LogModel::~LogModel()
{}




/*******************************************************************
	get_title
********************************************************************/
QString		LogModel::get_title( int row )
{
	if( row >= log_list.size() )
	{
		//ERRLOG( "row=%d, size=%d", row, log_list.size() )
		return	QString("");
	}

	return	log_list[row].title;
}


/*******************************************************************
	rowCount
********************************************************************/
int		LogModel::rowCount( const QModelIndex &parent ) const 
{
	return	log_list.size();
}




/*******************************************************************
	columnCount
********************************************************************/
int		LogModel::columnCount( const QModelIndex &parent ) const 
{ 
    return	head_list.size();
}





/*******************************************************************
	data
********************************************************************/
QVariant	LogModel::data( const QModelIndex &index, int role ) const
{
	int		col		=	index.column();
	int		row		=	index.row();

	if (role == Qt::DisplayRole)
	{
		//return	QString("col=%1,row=%2").arg(col).arg(row);
		switch(col)
		{
			case 0:
				return	log_list[row].title;
			case 1:
				return	log_list[row].author;
			case 2:
				return	log_list[row].date;
		}

	}
	return QVariant();
}




/*******************************************************************
	get_commit
********************************************************************/
QString		LogModel::get_commit( int row )
{
	if( row >= log_list.size() )
	{
		//ERRLOG( "log_list.size()=%d,row=%d", log_list.size(), row )
		return	QString("");
	}
	
	return	log_list[row].commit;
}



/*******************************************************************
	get_default_index
********************************************************************/
QModelIndex		LogModel::get_default_index()
{
	//if( log_list.size() == 0 )
		//ERRLOG("log_list.size() = %d", log_list.size() )
	
	return	createIndex(0,0);
}



/*******************************************************************
	set_root_path
********************************************************************/
void    LogModel::set_root_path( QString path )
{
	root_path	=	path;

	GitLog	git_log;

	// test for git graph
	git_log.get_log_graph( root_path );

	log_list	=	git_log.get_log_list( root_path );
    
    QModelIndex left_top        =   createIndex( 0, 0);
    QModelIndex right_bottom    =   createIndex( log_list.size(), head_list.size() );
    
    emit dataChanged( left_top, right_bottom );
	emit refresh_signal();
}






/*******************************************************************
	headerData
********************************************************************/
QVariant	LogModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
	if ( role == Qt::DisplayRole )
	{
		if ( orientation == Qt::Horizontal )
            return  head_list[section];
	}
	return QVariant();
}

