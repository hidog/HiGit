#include "syncdialog.h"
#include "ui_syncdialog.h"




/*******************************************************************
	SyncDialog
********************************************************************/
SyncDialog::SyncDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SyncDialog)
{
    ui->setupUi(this);
}



/*******************************************************************
	SyncDialog
********************************************************************/
SyncDialog::~SyncDialog()
{
    delete ui;
}



/*******************************************************************
	closeEvent
********************************************************************/
void	SyncDialog::closeEvent( QCloseEvent *event )
{
	deleteLater();
}
