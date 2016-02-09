#include "filewidget.h"
#include "ui_filewidget.h"

#include "../model/file_model.h"
#include <QDebug>



/*******************************************************************
	FileWidget
********************************************************************/
FileWidget::FileWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileWidget)
{
    ui->setupUi(this);

	model	=	new FileModel( this );

	ui->fileTView->setModel( model );

	set_connect();
}




/*******************************************************************
	refresh_view_slot
********************************************************************/
void	FileWidget::refresh_view_slot()
{
	ui->fileTView->setModel(0);
	ui->fileTView->setModel( model );
}


/*******************************************************************
	set_connect
********************************************************************/
void	FileWidget::set_connect()
{
	connect(	ui->fileTView,		SIGNAL(doubleClicked(const QModelIndex&)),			this,		SLOT(double_clicked_slot(const QModelIndex&))	);
	connect(	this,				SIGNAL(enter_dir_signal(const QModelIndex&)),		model,		SLOT(enter_dir_slot(const QModelIndex&))		);
	connect(	model,				SIGNAL(refresh_signal()),							this,		SLOT(refresh_view_slot())						);
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


