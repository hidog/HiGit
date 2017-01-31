#include "log_model.h"

#include <QDebug>
#include <QImage>
#include <QPainter>
#include "../src/git_cmd/git_log.h"
#include "../src/def.h"


/*******************************************************************
	LogModel
********************************************************************/
LogModel::LogModel( QObject *parent ) :
	QAbstractTableModel( parent )
{
	head_list << "graph" << "title" << "author" << "date";
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
	text_data
********************************************************************/
QVariant	LogModel::text_data( const QModelIndex &index, int role ) const
{
	int		col		=	index.column();
	int		row		=	index.row();

	if (role != Qt::DisplayRole)
		assert(0);
	
	//return	QString("col=%1,row=%2").arg(col).arg(row);
	switch(col)
	{
		case 1:
			return	log_list[row].title;
		case 2:
			return	log_list[row].author;
		case 3:
			return	log_list[row].date;
	}

	return QVariant();
}



/*******************************************************************
	graph_data
********************************************************************/
QVariant	LogModel::graph_data( const QModelIndex &index, int role ) const
{
	int		row		=	index.row();
	int		col		=	index.column();

	if( col == 0 )
	{
		if( row < graph_list.size() )
			return	graph_list[row];
		else
			return	QVariant();
	}
	else
		return	QVariant();
}



/*******************************************************************
	data
********************************************************************/
QVariant	LogModel::data( const QModelIndex &index, int role ) const
{
	QVariant	var	=	QVariant();	

	switch( role )
	{
		case Qt::DisplayRole:
			var	=	text_data( index, role );
			break;
		case Qt::DecorationRole:
			var	=	graph_data( index, role );		
			break;
	}

	return	var;


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
	int		log_count	=	0;

	root_path	=	path;
	GitLog			git_log;
	QList<int>		node_list;

	// get git log graph
	log_count	=	git_log.get_log_graph( root_path, line_list, node_list );
	// draw git log graph
	draw_git_log_graph( node_list, log_count );

	// get log 
	log_list	=	git_log.get_log_list( root_path );
    
    QModelIndex left_top        =   createIndex( 0, 0);
    QModelIndex right_bottom    =   createIndex( log_list.size(), head_list.size() );
    
    emit dataChanged( left_top, right_bottom );
	emit refresh_signal();
}



/*******************************************************************
	draw_git_log_graph
********************************************************************/
void	LogModel::draw_git_log_graph( QList<int> &node_list, int node_size )
{
	graph_list.clear();

	int			width	=	line_list.size() * 21;
	int			height	=	21;
	QImage		img( width, height, QImage::Format_RGB888 );
	//QPainter	painter(&img);
	int			i,	j;
	int			node_index;

	GitLineList::iterator	itr;

	// draw image
	for( i = 0; i < node_size; i++ )
	{
		node_index	=	node_list[i];

		img.fill( Qt::red);
		QPainter	painter(&img);
		painter.drawEllipse( node_index*7, 7, 5, 5 );
		graph_list.push_back( img );
	}

	qDebug() << graph_list.size();
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

