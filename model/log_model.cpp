#include "log_model.h"

#include <QDebug>
#include "../src/git_cmd/git_log.h"


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
	set_root_path
********************************************************************/
void    LogModel::set_root_path( QString path )
{
	root_path	=	path;

	GitLog	git_log;
	log_list	=	git_log.get_log_list( root_path );

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

