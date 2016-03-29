#include "filewidget.h"
#include "ui_filewidget.h"

#include "../model/file_model.h"
#include "adddialog.h"

#include <QDebug>
#include <QMenu>
#include <QItemSelectionModel>
#include <QMouseEvent>


/*******************************************************************
	FileWidget
********************************************************************/
FileWidget::FileWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileWidget),
	right_menu(NULL),
	add_action(NULL)
{
	int		i;

    ui->setupUi(this);

	model	=	new FileModel( this );
	ui->fileTView->setModel( model );

	header_width_vec.resize( model->get_header_count() );

	//header_width_vec[0]	=	;	don't set icon width.
	//ui->fileTView->horizontalHeader()->setSectionResizeMode( 0, QHeaderView::Fixed );  need set after refresh.
	header_width_vec[1]	=	300;
	header_width_vec[2]	=	80;
	header_width_vec[3]	=	70;
	header_width_vec[4] =	60;
    //header_width_vec[5] =   200;

	set_right_menu();
	set_connect();
}



/*******************************************************************
	set_right_menu
********************************************************************/
void FileWidget::right_menu_slot( const QPoint &pos )
{
	right_menu->exec( QCursor::pos() ); 
}




/*******************************************************************
	set_right_menu
********************************************************************/
void	FileWidget::set_right_menu()
{
	if( right_menu != NULL )
	{
		delete	right_menu;
		right_menu	=	NULL;
	}
	if( add_action != NULL )
	{
		delete	add_action;
		add_action	=	NULL;
	}

	//
	ui->fileTView->setContextMenuPolicy(Qt::CustomContextMenu); 	  

	right_menu	=	new QMenu( ui->fileTView );
	add_action	=	new QAction( "Add", right_menu );
 
	right_menu->addAction( add_action );  

	connect(	ui->fileTView,	SIGNAL(customContextMenuRequested(QPoint)),		this,	SLOT(right_menu_slot(QPoint))	);
	connect(	add_action,		SIGNAL(triggered()),							this,	SLOT(add_slot())				);
}


/*******************************************************************
	refresh_view_slot
********************************************************************/
void	FileWidget::refresh_view_slot()
{
	int		i;

	ui->fileTView->setModel( NULL );
	ui->fileTView->setModel( model );

	ui->fileTView->horizontalHeader()->setSectionResizeMode( 0, QHeaderView::Fixed );

	for( i = 1; i < header_width_vec.size(); i++ )
		ui->fileTView->setColumnWidth( i, header_width_vec[i] );

}



/*******************************************************************
	refresh_view_slot
********************************************************************/
void	FileWidget::header_resize_slot( int index, int old_size, int new_size )
{
	assert( index < header_width_vec.size() );
	assert( index != 0 );

	header_width_vec[index]		=	new_size;
}


/*******************************************************************
	set_connect
********************************************************************/
void	FileWidget::set_connect()
{
	connect(	ui->fileTView,		SIGNAL(doubleClicked(const QModelIndex&)),			this,		SLOT(double_clicked_slot(const QModelIndex&))	);
	connect(	this,				SIGNAL(enter_dir_signal(const QModelIndex&)),		model,		SLOT(enter_dir_slot(const QModelIndex&))		);
	connect(	model,				SIGNAL(refresh_signal()),							this,		SLOT(refresh_view_slot())						);
	connect(	model,				SIGNAL(path_change_signal(QString)),				this,		SLOT(path_change_slot(QString))					);
	connect(	ui->refreshButton,	SIGNAL(clicked()),									model,		SLOT(refresh_slot())							);
    connect(    ui->addButton,      SIGNAL(clicked()),                                  this,       SLOT(add_slot())                                );

	connect(	ui->fileTView->horizontalHeader(),		SIGNAL(sectionResized(int,int,int)),		this,		SLOT(header_resize_slot(int,int,int))		);
}




/*******************************************************************
    add_slot
********************************************************************/
void    FileWidget::add_slot()
{
	QItemSelectionModel	*selecteds		=	ui->fileTView->selectionModel();
	QModelIndexList 	row_list_tmp	=	selecteds->selectedIndexes();
	QModelIndexList		row_list;

	bool	is_choosed;

	// remove repeat row.
	row_list.clear();
	foreach( QModelIndex index, row_list_tmp )
	{
		is_choosed	=	false;
		foreach( QModelIndex index2, row_list )
		{
			if( index.row() == index2.row() )
			{
				is_choosed	=	true;
				break;
			}
		}

		if( is_choosed == false )
			row_list.push_back( index );
	}

	QFileInfoList	add_list	=	model->get_add_selected_list( row_list );

	//foreach( QFileInfo info, add_list )
		//qDebug() << info.fileName();

	AddDialog	*add_dialog		=	new AddDialog( root_path, add_list, this );
	add_dialog->show();
}



/*******************************************************************
	path_change_slot
********************************************************************/
void	FileWidget::path_change_slot( QString path )
{
	QString		str;
	int			index	=	0;

	// need disconnect. otherwise, model will get more signals.
	disconnect(		ui->pathCBox,		SIGNAL(currentIndexChanged(const QString&)),		model,		SLOT(path_change_slot(const QString&))			);

	//
	ui->pathCBox->clear();
	while(true)
	{
		index	=	path.indexOf( "/", index );
		if( index < 0 )
			break;

		str		=	path.mid( 0, index+1 );
		index++;

		ui->pathCBox->addItem( str );
	}

	// set to last index.
	int		last_index	=	ui->pathCBox->count() - 1;
	ui->pathCBox->setCurrentIndex( last_index );

	//
	connect(		ui->pathCBox,		SIGNAL(currentIndexChanged(const QString&)),		model,		SLOT(path_change_slot(const QString&))			);
}




/*******************************************************************
	double_clicked_slot
********************************************************************/
void	FileWidget::double_clicked_slot( const QModelIndex &index )
{
	emit enter_dir_signal(index);		
}





/*******************************************************************
	FileWidget
********************************************************************/
FileWidget::~FileWidget()
{
    delete	model;			model		=	NULL;
    delete	ui;				ui			=	NULL;
	delete	add_action;		add_action	=	NULL;
	delete	right_menu;		right_menu	=	NULL;
}





/*******************************************************************
	set_root_path
********************************************************************/
void	FileWidget::set_root_path( QString _path )
{
	root_path	=	_path;
    
    model->set_root_path( root_path );
    model->init_file_list();
}


