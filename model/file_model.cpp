#include "file_model.h"

#include <QDebug>



/*******************************************************************
	FileWidget
********************************************************************/
FileModel::FileModel( QObject *parent ) :
	QAbstractTableModel( parent )
{
    head_list << " " << "path" << "status" << "extends";
}




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
    return	file_list.size();
}




/*******************************************************************
	columnCount
********************************************************************/
int		FileModel::columnCount( const QModelIndex &parent ) const 
{ 
    return	head_list.size();;
}




/*******************************************************************
	data
********************************************************************/
QVariant	FileModel::data( const QModelIndex &index, int role ) const
{
	if (role == Qt::DisplayRole)
	{
        switch( index.column() )
        {
            case 0:
                return QString(" ");
            case 1:
                return file_list[index.row()].fileName();
            case 2:
                return QString("test");//file_list[index.row()].extends();
            case 3:
                return QString("status");
        }
        
        //return  file_list[index.row()].fileName();
        
       /* return QString("Row%1, Column%2")
				   .arg(index.row() + 1)
				   .arg(index.column() +1);*/
	}
	return QVariant();
}



/*******************************************************************
	set_root_path
********************************************************************/
void    FileModel::set_root_path( QString path )
{
    root_path   =   path;
    dir.setPath( root_path );
}



/*******************************************************************
	init_file_list
********************************************************************/
void    FileModel::init_file_list()
{
    file_list   =   dir.entryInfoList();
    
    QModelIndex     top_left    =   createIndex( 1, 1 );
    emit dataChanged( top_left, top_left );
    
    //QList<QFileInfo>::iterator  itr;
    
    //for( itr = file_list.begin(); itr != file_list.end(); ++itr )
      //  qDebug() << (*itr).fileName();
}



/*******************************************************************
	headerData
********************************************************************/
QVariant	FileModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
	if ( role == Qt::DisplayRole )
	{
		if ( orientation == Qt::Horizontal )
            return  head_list[section];
	}
	return QVariant();
}

