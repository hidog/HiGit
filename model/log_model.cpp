#include "log_model.h"

#include <QDebug>
#include "../src/git_cmd/git_log.h"


/*******************************************************************
	LogModel
********************************************************************/
LogModel::LogModel( QObject *parent ) :
	QAbstractTableModel( parent )
{}




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
	return	3;
}




/*******************************************************************
	columnCount
********************************************************************/
int		LogModel::columnCount( const QModelIndex &parent ) const 
{ 
    return	3;
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
		return	QString("col=%1,row=%2").arg(col).arg(row);
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
	git_log.test( root_path );
}






/*******************************************************************
	headerData
********************************************************************/
QVariant	LogModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
	if ( role == Qt::DisplayRole )
	{
		if ( orientation == Qt::Horizontal )
            return  QString("test");
	}
	return QVariant();
}

