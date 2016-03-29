#include "adddialog.h"
#include "ui_adddialog.h"

#include <QDebug>
#include <QDir>
#include <QCheckBox>

#include "../src/tools.h"
#include "../src/git_cmd/git_status.h"

#include <cassert>

/*******************************************************************
    AddDialog
********************************************************************/
AddDialog::AddDialog( QString _root_path, QFileInfoList _add_list, QWidget *parent ) :
    QDialog(parent),
    ui(new Ui::AddDialog),
	add_list(_add_list),
	root_path(_root_path)
{
    ui->setupUi(this);

	assert( ui->addTWidget->columnCount() >= 2 );

	ui->addTWidget->setColumnWidth( 0, 20 );
	ui->addTWidget->setColumnWidth( 1, 300 );

	ui->untrackedTWidget->setColumnWidth( 0, 20 );
	ui->untrackedTWidget->setColumnWidth( 1, 300 );

	set_add_files();
	set_untracked_files();
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
    set_table_widget
********************************************************************/
void	AddDialog::set_table_widget( QTableWidget *table, const QFileInfoList &list, Qt::CheckState check_state )
{
	int			count;
	int			column_count;
	QString		filename;
	QString		extension;
	QString		filesize;
	QDir		root_dir( root_path );

	QTableWidgetItem	*item	=	NULL;
	QCheckBox			*cbox	=	NULL;

	//
	table->setRowCount( list.size() );
	count	=	0;

	foreach( QFileInfo info, list )
	{
		column_count	=	0;

		filename	=	root_dir.relativeFilePath( info.absoluteFilePath() );
		extension	=	get_extension( info.fileName() );
		filesize	=	get_filesize_str( info.size() );

		cbox	=	new QCheckBox( "", table );
		cbox->setCheckState( check_state );
		connect(	cbox,	SIGNAL(stateChanged(int)),		this,	SLOT(cbox_state_change_slot(int))	);
		table->setCellWidget( count, column_count++, cbox );

		item	=	new QTableWidgetItem( filename );
		table->setItem( count, column_count++, item );
        
        item    =   new QTableWidgetItem( extension );
        table->setItem( count, column_count++, item );

		item    =   new QTableWidgetItem( filesize );
        table->setItem( count, column_count++, item );

		count++;
	}
}




/*******************************************************************
    cbox_state_change_slot
********************************************************************/
void	AddDialog::cbox_state_change_slot( int state )
{
	QCheckBox	*cbox	=	static_cast<QCheckBox*>(sender());

	if( cbox->parent()->parent() == ui->addTWidget )
		qDebug() << "addTWidget";
	else if( cbox->parent()->parent() == ui->untrackedTWidget )
		qDebug() << "untrackedTWidget";
}




/*******************************************************************
    set_untracked_files
********************************************************************/
void	AddDialog::set_untracked_files()
{
	GitStatus		git_status;
	QFileInfoList	list;

	list	=	git_status.get_untracked_files( root_path );

	// remove repeat element in add_list.
	foreach( QFileInfo info, list )
	{
		if( add_list.contains( info ) )
			list.removeOne(info);
	}

	//
	set_table_widget( ui->untrackedTWidget, list, Qt::Unchecked );
	
	ui->untrackedTWidget->sortItems( 2, Qt::AscendingOrder );
}



/*******************************************************************
    set_add_files
********************************************************************/
void	AddDialog::set_add_files()
{
	set_table_widget( ui->addTWidget, add_list, Qt::Checked );
}
