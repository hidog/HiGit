#include "logwidget.h"
#include "ui_logwidget.h"

#include "../model/log_model.h"
#include <QDebug>
#include <QAbstractTableModel>





/*******************************************************************
	LogWidget
********************************************************************/
LogWidget::LogWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogWidget)
{
    ui->setupUi(this);

	model	=	new LogModel(this);
	ui->logTView->setModel(model);

    connect(    model,          SIGNAL(refresh_signal()),                   this,   SLOT(refresh_view_slot())                   );
	connect(	ui->logTView,	SIGNAL(pressed(const QModelIndex&)),		this,	SLOT(select_slot(const QModelIndex&))		);
}





/*******************************************************************
	LogWidget
********************************************************************/
LogWidget::~LogWidget()
{
    delete model;
    delete ui;
}



/*******************************************************************
	refresh_view_slot
 ********************************************************************/
void	LogWidget::refresh_view_slot()
{
    ui->logTView->setModel( NULL );
    ui->logTView->setModel( model );
    
    //ui->fileTView->setColumnWidth( 1, 300 );
    //ui->fileTView->setColumnWidth( 2, 80 );
    //ui->fileTView->setColumnWidth( 3, 70 );
}




/*******************************************************************
	select_slot
********************************************************************/
void	LogWidget::select_slot( const QModelIndex& index )
{
	QString			title	=	model->get_title(index.row());
	ui->titleTBrowser->setText(title);

	GitLog			git_log;
	QString			commit		=	model->get_commit(index.row());
	QStringList		file_list	=	git_log.get_file_list( root_path, commit );

	ui->fileLWidget->clear();
	ui->fileLWidget->addItems(file_list);
}




/*******************************************************************
	LogWidget
********************************************************************/
void	LogWidget::set_root_path( QString path )
{
	root_path	=	path;
	model->set_root_path(root_path);

	QModelIndex		index	=	model->get_default_index();
	select_slot( index );
	//ui->logTView->setCurrentIndex( index );	
}


