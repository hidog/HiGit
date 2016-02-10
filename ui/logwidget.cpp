#include "logwidget.h"
#include "ui_logwidget.h"

#include "../model/log_model.h"




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
}





/*******************************************************************
	LogWidget
********************************************************************/
LogWidget::~LogWidget()
{
    delete ui;
}





/*******************************************************************
	LogWidget
********************************************************************/
void	LogWidget::set_root_path( QString path )
{
	root_path	=	path;
	model->set_root_path(root_path);
}


