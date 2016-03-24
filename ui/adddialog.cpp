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
	ui->addTWidget->setRowCount( add_list.size() );
	ui->addTWidget->setColumnCount( 2 );


	QTableWidgetItem	*item	=	NULL;

	for( int i = 0; i < add_list.size(); i++ )
	{
		item	=	new QTableWidgetItem( add_list[i].fileName() );
		ui->addTWidget->setItem( i, 0, item );
        
        item    =   new QTableWidgetItem( add_list[i].size() );
        ui->addTWidget->setItem( i, 1, item );
	}

}
