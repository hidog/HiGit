#include "file_model.h"

#include <QDebug>
#include "../src/git_cmd/git_status.h"


/*******************************************************************
	FileWidget
********************************************************************/
FileModel::FileModel( QObject *parent ) :
	QAbstractTableModel( parent )
{
    head_list << " " << "path" << "status" << "extends";
	
	last_index	=	createIndex( 0, 0 );
	
	dir.setSorting( QDir::Name | QDir::DirsFirst );
	dir.setFilter( QDir::NoDot | QDir::Dirs | QDir::Files );
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
	return	file_list.size() + delete_file_list.size();
}




/*******************************************************************
	columnCount
********************************************************************/
int		FileModel::columnCount( const QModelIndex &parent ) const 
{ 
    return	head_list.size();
}




/*******************************************************************
	refresh_view
	這邊會紀錄上一次的file_size
	取max, 避免有網格沒更新到.
	(例如從數量多的folder移動到數量少的)
	P.S. 畫面更新沒測試過,有空在測試,包含效能的影響.
********************************************************************/
void	FileModel::refresh_view()
{
	int		row		=	file_list.size() > last_index.row() ? file_list.size() : last_index.row();;
	int		col		=	head_list.size();

	QModelIndex		left_top		=	createIndex( 0, 0 );
	QModelIndex		right_bottom	=	createIndex( row, col );

	emit dataChanged( left_top, right_bottom );
	emit refresh_signal();

	last_index	=	createIndex( file_list.size(), col );

}





/*******************************************************************
	get_file_list
********************************************************************/
QFileInfoList	FileModel::get_file_list()
{
	QFileInfoList	list	=	dir.entryInfoList();

	if( dir.path() == root_path )
		list.removeAt(0);

	return	list;
}




/*******************************************************************
	enter_dir
********************************************************************/
void	FileModel::enter_dir_slot( const QModelIndex &index )
{
	int			row		=	index.row();
	QFileInfo	info	=	file_list[row];
	QString		path	=	info.fileName();
	GitStatus	git_status;

	//qDebug() << path;

	if( file_list[row].isDir() )
	{
		if( path == QString("..") )
			dir.cdUp();
		else
			dir.cd(path);

		delete_file_list	=	git_status.get_delete_files( dir.path() );
		file_list	=	get_file_list();


		/*QList<QFileInfo>::Iterator	itr;
		for( itr = file_list.begin(); itr != file_list.end(); ++itr )
			qDebug() << itr->fileName();*/

		refresh_view();
	}
}



/*******************************************************************
	data
********************************************************************/
QVariant	FileModel::data( const QModelIndex &index, int role ) const
{
	int		col		=	index.column();
	int		row		=	index.row();

	//qDebug () << col << " " << row;

	QString		filename	=	row < file_list.size() ? file_list[row].fileName() : delete_file_list.at( row - file_list.size() );

	if (role == Qt::DisplayRole)
	{
        switch( col )
        {
            case 0:
                return QString("%1").arg(row);
            case 1:
				return	filename;
            case 2:
				if( row < file_list.size() )
				{
					if( file_list[row].isFile() == true )
					{
						GitStatus	git_status;
						return	git_status.get_file_status( dir.path(), filename );
					}
					else
						return	QString("dir");
				}
				else
				{
					GitStatus	git_status;
					return	git_status.get_file_status( dir.path(), filename );
				}
				

            case 3:
				if( row < file_list.size() )
				{
					if( file_list[row].isDir() == true )
						return	QString("dir");
					else
						return	get_extension( filename );
				}
				else
					return	get_extension( filename );
        }
	}
	return QVariant();
}




/*******************************************************************
	get_extension
********************************************************************/
QString		FileModel::get_extension( QString filename ) const
{
	int		index	=	filename.lastIndexOf('.');
	if( index < 0 )
		return	QString("");
	else
		return	filename.mid( index+1 );

	//return	QString("");
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
	GitStatus	git_status;

	delete_file_list	=	git_status.get_delete_files( dir.path() );
	file_list			=	get_file_list();

	/*QList<QFileInfo>::Iterator	itr;
    for( itr = file_list.begin(); itr != file_list.end(); ++itr )
		qDebug() << itr->fileName();*/

	refresh_view();
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

