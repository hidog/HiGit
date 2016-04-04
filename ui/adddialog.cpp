#include "adddialog.h"
#include "ui_adddialog.h"

#include <QDebug>
#include <QDir>
#include <QCheckBox>

#include <boost/thread.hpp>

#include "../src/tools.h"
#include "../src/git_cmd/git_status.h"
#include "../src/git_control.h"

#include <cassert>





/*******************************************************************
    AddDialog
********************************************************************/
AddDialog::AddDialog( QString _root_path, QFileInfoList _add_list, QWidget *parent ) :
    QDialog(parent),
    ui(new Ui::AddDialog),
	add_list(_add_list),
	root_path(_root_path),
	is_closed(false)
{
    ui->setupUi(this);

	connect(	ui->buttonBox,	SIGNAL(accepted()),		this,	SLOT(accepted_slot())	);

	assert( ui->addTWidget->columnCount() >= 2 );

	ui->addTWidget->setColumnWidth( 0, 20 );
	ui->addTWidget->setColumnWidth( 1, 300 );

	ui->untrackedTWidget->setColumnWidth( 0, 20 );
	ui->untrackedTWidget->setColumnWidth( 1, 300 );

	set_add_files();

	connect(	this,	SIGNAL(finish_untracked_files_signal(QList<QFileInfo>)),	this,	SLOT(finish_untracked_files_slot(QList<QFileInfo>))	);
	boost::thread	thr		=	boost::thread( boost::bind( &AddDialog::set_untracked_files, this ) );
}



/*******************************************************************
    accepted_slot
********************************************************************/
void	AddDialog::accepted_slot()
{
	QList<QString>	list;

	int			row_count	=	ui->addTWidget->rowCount();
	int			row;
	QString		str;

	assert( ui->addTWidget->horizontalHeaderItem(1)->text() == "filename" );

	// get filenames from ui.
	for( row = 0; row < row_count; row++ )
	{
		str		=	ui->addTWidget->item( row, 1 )->text();
		list.push_back( str );
	}

	// git add
	GitControl	git_control;
	git_control.add( root_path, list );
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
	is_closed	=	true;
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

		cbox	=	new QCheckBox( "" );
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
    search_checkbox
********************************************************************/
int		AddDialog::search_checkbox( QCheckBox *cbox, QTableWidget *table )
{
	assert( cbox != NULL && table != NULL );
	if( cbox == NULL || table == NULL )
		return	-1;

	// search
	int		row;
	int		row_count	=	table->rowCount();
	for( row = 0; row < row_count; row++ )
	{
		if( cbox == table->cellWidget( row, 0 ) )
			break;
	}

	return	row;
}


/*******************************************************************
    insert_table_item
********************************************************************/
void	AddDialog::insert_table_item( int row, QTableWidget *table )
{
	assert( ui->addTWidget->columnCount() == ui->untrackedTWidget->columnCount() );

	QTableWidget	*insert_table	=	NULL;
	Qt::CheckState	state			=	Qt::Unchecked;

	if( table == ui->addTWidget )
	{
		insert_table	=	ui->untrackedTWidget;
		state			=	Qt::Unchecked;
	}
	else if( table == ui->untrackedTWidget )
	{
		insert_table	=	ui->addTWidget;
		state			=	Qt::Checked;
	}
	else
		assert(0);

	if( insert_table == NULL )
		ERRLOG("insert_table is NULL.");

	// insert items.
	int		row_count	=	insert_table->rowCount();
	insert_table->insertRow(row_count);

	// insert checkbox.
	QCheckBox	*cbox	=	new QCheckBox("");
	cbox->setCheckState( state );
	connect(	cbox,	SIGNAL(stateChanged(int)),		this,	SLOT(cbox_state_change_slot(int))	);
	insert_table->setCellWidget( row_count, 0, cbox );

	// insert texts.
	int			col;
	int			col_count	=	insert_table->columnCount();
	QString		str;
	QTableWidgetItem	*item	=	NULL;
	for( col = 1; col < col_count; col++ )
	{
		str		=	table->item( row, col )->text();	// befareful that row is table's row, row_count is insert_table's row_count.
		item	=	new QTableWidgetItem( str );
		insert_table->setItem( row_count, col, item );
	}
}


/*******************************************************************
    cbox_state_change_slot
********************************************************************/
void	AddDialog::cbox_state_change_slot( int state )
{
	QCheckBox		*cbox	=	static_cast<QCheckBox*>(sender());
	QTableWidget	*table	=	NULL;

	if( cbox->parent()->parent() == ui->addTWidget )
		table		=	ui->addTWidget;
	else if( cbox->parent()->parent() == ui->untrackedTWidget )
		table		=	ui->untrackedTWidget;

	//
	int		row		=	search_checkbox( cbox, table );

	// add and remove item.
	insert_table_item( row, table );

	// remove
	table->removeRow( row );
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

	// note: avoid call signal after close.
	if( is_closed == false )
		emit	finish_untracked_files_signal(list);

	//set_table_widget( ui->untrackedTWidget, list, Qt::Unchecked );	
	//ui->untrackedTWidget->sortItems( 2, Qt::AscendingOrder );
}


/*******************************************************************
    finish_untracked_files_slot
********************************************************************/
void	AddDialog::finish_untracked_files_slot( QFileInfoList list )
{
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
