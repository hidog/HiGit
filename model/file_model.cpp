#include "file_model.h"



/*******************************************************************
	FileWidget
********************************************************************/
FileModel::FileModel( QObject *parent ) :
	QAbstractTableModel( parent )
{}




/*******************************************************************
	FileWidget
********************************************************************/
FileModel::~FileModel()
{}





/*******************************************************************
	rowCount
********************************************************************/
int		FileModel::rowCount( const QModelIndex &parent ) const 
{ 
	return	3; 
}




/*******************************************************************
	columnCount
********************************************************************/
int		FileModel::columnCount( const QModelIndex &parent ) const 
{ 
	return	3; 
}




/*******************************************************************
	data
********************************************************************/
QVariant	FileModel::data(const QModelIndex &index, int role ) const 
{
	if (role == Qt::DisplayRole)
	{
	   return QString("Row%1, Column%2")
				   .arg(index.row() + 1)
				   .arg(index.column() +1);
	}
	return QVariant();
}




/*******************************************************************
	data
********************************************************************/
QVariant	FileModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
	if (role == Qt::DisplayRole)
	{
		if (orientation == Qt::Horizontal) 
		{
			switch (section)
			{
				case 0:
					return QString("first");
				case 1:
					return QString("second");
				case 2:
					return QString("third");
			}
		}
	}
	return QVariant();
}

