#include "filewidget.h"
#include "ui_filewidget.h"

#include "../model/file_model.h"





/*******************************************************************
	FileWidget
********************************************************************/
FileWidget::FileWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileWidget)
{
    ui->setupUi(this);

	model	=	new FileModel( this );
	//ui->fileTView->setModel( model );
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
    
	ui->fileTView->setModel( model );
    //this->repaint();
}


