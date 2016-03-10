#include "file_model.h"

#include <QDebug>
#include <QFileIconProvider>

#include "../src/git_cmd/git_status.h"


/*******************************************************************
	FileWidget
********************************************************************/
FileModel::FileModel( QObject *parent ) :
	QAbstractTableModel( parent )
{
    head_list << " " << "file name" << "status" << "extends" << "size";
	
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
	get_header_count
********************************************************************/
int		FileModel::get_header_count()
{
	return	head_list.size();
}


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

		file_list	=	get_file_list();

		/*QList<QFileInfo>::Iterator	itr;
		for( itr = file_list.begin(); itr != file_list.end(); ++itr )
			qDebug() << itr->fileName();*/

		refresh_view();
	}
}



/*******************************************************************
	text_data
********************************************************************/
QVariant	FileModel::text_data( const QModelIndex &index, int role ) const
{
	int		col		=	index.column();
	int		row		=	index.row();

	QString		filename	=	file_list[row].fileName();
	QVariant	result;
	GitStatus	git_status;

	// handle DisplayRole only.
	if( role != Qt::DisplayRole )
		assert(false);

	switch( col )
	{
		case 1:
			result	=	filename;
			break;
		case 2:
			if( file_list[row].isFile() == true )
				result	=	git_status.get_file_status( dir.path(), filename );				
			else
				result	=	QString("dir");
			break;		
		case 3:
			if( file_list[row].isDir() == true )
				result	=	QString("dir");
			else
				result	=	get_extension( filename );
			break;
		case 4:
			if( file_list[row].isFile() == true )
				result	=	get_filesize_str( file_list[row].size() );
			else
				result	=	QString("");
			break;
	}

	return	result;
}



/*******************************************************************
	get_filesize_str
********************************************************************/
QString		FileModel::get_filesize_str( qint64 size ) const
{
	int			count	=	0;
	double		d		=	size;
	QString		unit	=	QString();
	QString		str;

	while( d > 1024 )
	{
		d	/=	1024;
		count++;
	}

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

	if( count == 0 )
		str		=	str.sprintf( "%lld %s", size, unit.toStdString().c_str() );
	else
		str		=	str.sprintf( "%.1lf %s", d, unit.toStdString().c_str() );

	return	str;
}



/*******************************************************************
	icon_data
********************************************************************/
QVariant	FileModel::icon_data( const QModelIndex &index, int role ) const
{
	int		col		=	index.column();
	int		row		=	index.row();

	QString		filename	=	file_list[row].fileName();

	assert( row < file_list.size() );

	QFileInfo			info	=	 file_list[row]; //delete_file_list.at( row - file_list.size() );
	QFileIconProvider	icon_pv;
	QVariant			result;

	// handle DecorationRole only.
	if( role != Qt::DecorationRole )
		assert(false);

	switch( col )
	{
		case 0:
			result	=	icon_pv.icon(info);
			break;
	}

	return result;
}



/*******************************************************************
	data
********************************************************************/
QVariant	FileModel::data( const QModelIndex &index, int role ) const
{
	int		col		=	index.column();
	int		row		=	index.row();

	//qDebug () << col << " " << row;

	assert( row < file_list.size() );

	QString		filename	=	file_list[row].fileName();

	switch( role )
	{
		case Qt::DisplayRole:
			return	text_data( index, role );
		case Qt::DecorationRole:
			return	icon_data( index, role );		
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

