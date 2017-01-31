#include "file_model.h"

#include <QDebug>
#include <QFileIconProvider>
#include <QColor>
#include <QDateTime>
#include <QTimer>

//#include "../src/git_control.h"
#include "../src/tools.h"
#include "../src/git_cmd/git_status.h"
#include "../src/tools.h"

#include <boost/thread.hpp>



/*******************************************************************
	FileWidget
********************************************************************/
FileModel::FileModel( QObject *parent ) :
	QAbstractTableModel( parent ),
	thr(NULL),
	file_loop(false),
	file_loop_fisish(true),
	timer(NULL)
{
    head_list << " " << "file name" << "status" << "extends" << "size" ; //<< "last modified";
	
	last_index	=	createIndex( 0, 0 );
	
	dir.setSorting( QDir::Name | QDir::DirsFirst );
	dir.setFilter( QDir::NoDot | QDir::Dirs | QDir::Files );
}




/*******************************************************************
	FileWidget
********************************************************************/
FileModel::~FileModel()
{
	if( thr != NULL )
	{
		file_loop	=	false;
		thr->join();
		delete	thr;
		thr		=	NULL;
	}

	if( timer != NULL )
	{
		delete	timer;
		timer	=	NULL;
	}
}




/*******************************************************************
	refresh_slot
********************************************************************/
void	FileModel::refresh_slot()
{
	//qDebug() << __FUNCTION__;

	//
	if( thr != NULL )
	{
		if( file_loop_fisish == false )
			return;
		else
		{
			thr->join();
			delete	thr;
			thr		=	NULL;
		}		
	}

    // start loop to get all file status.
	file_loop	=	true;
	thr			=	new boost::thread( &FileModel::update_file_status, this );
}




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
	refresh_singal
********************************************************************/
void	FileModel::refresh_singal( int row )
{
	int		col		=	head_list.size();

	QModelIndex		left_top		=	createIndex( row, 0 );
	QModelIndex		right_bottom	=	createIndex( row, col );

	emit dataChanged( left_top, right_bottom );
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
	int		row		=	file_list.size() > last_index.row() ? file_list.size() : last_index.row();
	int		col		=	head_list.size();

	QModelIndex		left_top		=	createIndex( 0, 0 );
	QModelIndex		right_bottom	=	createIndex( row, col );

	emit dataChanged( left_top, right_bottom );
	emit refresh_signal();

	last_index	=	createIndex( file_list.size(), col );

}





/*******************************************************************
	get_file_list
    see update_file_status
********************************************************************/
void	FileModel::get_file_list()
{
	// 
	if( thr != NULL )
	{
		file_loop	=	false;
		thr->join();
		delete	thr;
		thr		=	NULL;
	}

	// get relative path.
	QString		relative_path	=	QDir(root_path).relativeFilePath( dir.path() );  //dir.relativeFilePath( root_path );
	if( relative_path[0] == '.' )
		relative_path[0]	=	'/';
	else
		relative_path.push_front('/');

	if( *(relative_path.end()-1) != '/' )
		relative_path.push_back('/');

	emit path_change_signal( relative_path );

	//
	file_list	=	dir.entryInfoList();

	if( dir.path() == root_path )
		file_list.removeAt(0);

	//
	status_vec.clear();
	status_vec.resize( file_list.size() );

    // start loop to get all file status.
	file_loop	=	true;
	thr			=	new boost::thread( &FileModel::update_file_status, this );
}




/*******************************************************************
	update_file_status
    use git status -s to get all file status.
    use single git status get chinese file name status.
    note: git status -s can't get chinese file name, but faster.
********************************************************************/
void	FileModel::update_file_status()
{
	//
	file_loop_fisish	=	false;

	int			row		=	0;

	GitStatus	git_status;
	QString		status;
	QColor		color;

	QFileInfoList::iterator		info_itr	=	file_list.begin();
	FileStatusVec::iterator		status_itr	=	status_vec.begin();

	while( file_loop )
	{
		if( info_itr == file_list.end() || status_itr == status_vec.end() )
			break;

		status	=	git_status.get_file_status( dir.path(), info_itr->fileName() );
		color	=	git_status.get_status_color( status );

		status_itr->status	=	status;
		status_itr->color	=	color;

		//qDebug() << row;
		refresh_singal( row++ );

		++info_itr;
		++status_itr;
	}

	//
	file_loop_fisish	=	true;
}



/*******************************************************************
	get_add_selected_list
	get selected by QTableView, and need added files.
********************************************************************/
QFileInfoList 	FileModel::get_add_selected_list( const QModelIndexList index_list ) const
{
	QFileInfoList	list;
	list.clear();

	int			row	=	0;
	QFileInfo	info;

	foreach( QModelIndex index, index_list )
	{
		row		=	index.row();

		if( status_vec[row].status == GIT_STATUS_UNTRACKED || status_vec[row].status == "" )
		{
			info	=	file_list.at(row);
			if( true == info.isFile() )
				list.push_back( info );
		}
	}

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

	if( info.isDir() )
	{
		if( path == QString("..") )
			dir.cdUp();
		else
			dir.cd(path);

		get_file_list();

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

	QFileInfo	info		=	file_list[row];
	QVariant	result;
	GitStatus	git_status;

	// handle DisplayRole only.
	if( role != Qt::DisplayRole )
		assert(false);

	switch( col )
	{
		case 1:
			result	=	info.fileName();
			break;
		case 2:
			if( info.isDir() == false )
				result	=	status_vec[row].status;
			else
				result	=	QString("dir");
			break;		
		case 3:
			if( info.isDir() == true )
				result	=	QString("dir");
			else
				result	=	get_extension( info.fileName() );		
			break;
		case 4:
			if( info.isDir() == false )
				result	=	get_filesize_str( info.size() );
			else
				result	=	QString("");
			break;
        case 5:
            if( info.isFile() == true)
                result  =   info.lastModified();
            break;
	}

	return	result;
}



/*******************************************************************
	icon_data
********************************************************************/
QVariant	FileModel::icon_data( const QModelIndex &index, int role ) const
{
	int		col		=	index.column();
	int		row		=	index.row();

	assert( row < file_list.size() );

	QFileInfo			info	=	file_list[row]; 
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

	QVariant	var	=	QVariant();

	assert( row < file_list.size() );

	QFileInfo	info		=	file_list[row];

	switch( role )
	{
		case Qt::DisplayRole:
			var	=	text_data( index, role );
			break;
		case Qt::DecorationRole:
			var	=	icon_data( index, role );		
			break;
		case Qt::TextColorRole:
            if( col < 3 )
                var	=	status_vec[row].color;
			break;
	}

	return var;
}



/*******************************************************************
	get_font_color
********************************************************************/
QVariant	FileModel::get_font_color( const QModelIndex &index, int role ) const
{
	int		col		=	index.column();
	int		row		=	index.row();

	QFileInfo	info	=	file_list[row];
	QVariant	result;
	GitStatus	git_status;

	// handle DisplayRole only.
	if( role != Qt::TextColorRole )
		assert(false);

	switch( col )
	{
		case 1:
		case 2:
			result	=	git_status.get_file_color( dir.path(), info.fileName() );			
			break;		
	}

	return	result;
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
	get_file_list();
	refresh_view();

	if( timer != NULL )
	{
		delete	timer;
		timer	=	NULL;
	}

	timer	=	new QTimer( this );
	connect(	timer,		SIGNAL(timeout()),		this,		SLOT(refresh_slot())	);
	//timer->setInterval( 1000 );
	timer->start( REFRESH_TIMEOUT );
}




/*******************************************************************
	path_change_slot
********************************************************************/
void	FileModel::path_change_slot( const QString &new_path )
{
	dir.cd( root_path + new_path );

	get_file_list();
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

