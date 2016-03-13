#include "filewidget.h"
#include "ui_filewidget.h"

#include "../model/file_model.h"
#include <QDebug>
#include <QMenu>


/*******************************************************************
	FileWidget
********************************************************************/
FileWidget::FileWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileWidget)
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


	// test for menu
	ui->fileTView->setContextMenuPolicy(Qt::CustomContextMenu); 	  
	//QTableView *tableview;  
	//QMenu *rightMenu;  // 右鍵菜單
	QAction *cutAction;  // 剪貼  
	QAction *copyAction;  // 複製  
	QAction *pasteAction;  // 貼上  
	QAction *deleteAction;  // 刪除 
	rightMenu = new QMenu;
	cutAction = new QAction("cut",this);  
	copyAction = new QAction("copy",this);  
	pasteAction = new QAction("past",this);  
	deleteAction = new QAction("delete",this);    
	rightMenu->addAction(cutAction);  
	rightMenu->addAction(copyAction);  
	rightMenu->addAction(pasteAction);  
	rightMenu->addAction(deleteAction);
	connect( ui->fileTView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(clicked_rightMenu(QPoint)) );

	set_connect();
}

void FileWidget::clicked_rightMenu(const QPoint &pos)
{
	rightMenu->exec(QCursor::pos()); 
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

	connect(	ui->fileTView->horizontalHeader(),		SIGNAL(sectionResized(int,int,int)),		this,		SLOT(header_resize_slot(int,int,int))		);
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
    delete model;
    delete ui;
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


