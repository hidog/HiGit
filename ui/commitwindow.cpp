#include "commitwindow.h"
#include "ui_commitwindow.h"




/*******************************************************************
	CommitWIndow
********************************************************************/
CommitWIndow::CommitWIndow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommitWIndow)
{
    ui->setupUi(this);
}




/*******************************************************************
	CommitWIndow
********************************************************************/
CommitWIndow::~CommitWIndow()
{
    delete ui;
}







/*******************************************************************
	closeEvent
********************************************************************/
void	CommitWIndow::closeEvent( QCloseEvent *event )
{
	deleteLater();
}

