#include "adddialog.h"
#include "ui_adddialog.h"

#include <QDebug>


/*******************************************************************
    AddDialog
********************************************************************/
AddDialog::AddDialog( QFileInfoList _add_list, QWidget *parent ) :
    QDialog(parent),
    ui(new Ui::AddDialog),
	add_list(_add_list)
{
    ui->setupUi(this);

	set_add_files();
}




/*******************************************************************
    AddDialog
********************************************************************/
AddDialog::~AddDialog()
{
    delete ui;
}





/*******************************************************************
    AddDialog
********************************************************************/
void	AddDialog::closeEvent( QCloseEvent *event )
{
	deleteLater();
}


/*******************************************************************
    set_add_files
********************************************************************/
void	AddDialog::set_add_files()
{
	ui->addTWidget->clear();

	// test code.
	ui->addTWidget->setRowCount( 100 );
	ui->addTWidget->setColumnCount( 1 );


	QTableWidgetItem	*item	=	NULL;

	for( int i = 0; i < 100; i++ )
	{
		item	=	new QTableWidgetItem( "test" );
		ui->addTWidget->setItem( i, 0, item );
	}

}
