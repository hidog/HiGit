#include "commitwindow.h"
#include "ui_commitwindow.h"

#include "../src/git_cmd/git_status.h"
#include "../src/tools.h"

#include <QDebug>
#include <QCheckBox>
#include <QDir>
#include <boost/thread.hpp>


/*******************************************************************
	CommitWIndow
********************************************************************/
CommitWindow::CommitWindow( QString _root_path, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CommitWindow),
	root_path(_root_path)
{
    ui->setupUi(this);

	ui->fileTWidget->setColumnWidth( 0, 20 );
	ui->fileTWidget->setColumnWidth( 1, 300 );

	connect(	this,	SIGNAL(finish_modify_list_signal(QList<FileStatus>)),		this,		SLOT(finish_modify_list_slot(QList<FileStatus>))	);

	boost::thread	thr		=	boost::thread( boost::bind( &CommitWindow::get_modify_list, this ) );
}




/*******************************************************************
	CommitWIndow
********************************************************************/
CommitWindow::~CommitWindow()
{
    delete ui;
}


/*******************************************************************
	finish_modify_list_slot
********************************************************************/
void	CommitWindow::finish_modify_list_slot( FileStatusList file_list )
{
	FileStatusList::iterator	itr;

	int			count, column_count;
	QString		extension, filesize;
	QDir		root_dir( root_path );
	QFileInfo	info;

	ui->fileTWidget->setRowCount( file_list.size() );
	QTableWidgetItem	*item	=	NULL;
	QCheckBox			*cbox	=	NULL;
	QTableWidget		*table	=	ui->fileTWidget;

	//
	count	=	0;
	for( itr = file_list.begin(); itr != file_list.end(); ++itr )
	{
		column_count	=	0;

		info		=	QFileInfo( root_path + "/" + itr->name );
		extension	=	get_extension( itr->name );
		filesize	=	get_filesize_str( info.size() );

		cbox	=	new QCheckBox( "" );

		if( itr->status != GIT_STATUS_UNTRACKED )
			cbox->setCheckState( Qt::Checked );
		else
			cbox->setCheckState( Qt::Unchecked );
		//connect(	cbox,	SIGNAL(stateChanged(int)),		this,	SLOT(cbox_state_change_slot(int))	);
		table->setCellWidget( count, column_count++, cbox );

		item	=	new QTableWidgetItem( itr->name );
		item->setForeground( itr->color );
		table->setItem( count, column_count++, item );

		item	=	new QTableWidgetItem( itr->status );
		item->setForeground( itr->color );
		table->setItem( count, column_count++, item );
        
        item    =   new QTableWidgetItem( extension );
        table->setItem( count, column_count++, item );

		item    =   new QTableWidgetItem( filesize );
        table->setItem( count, column_count++, item );

		count++;
	}
}


/*******************************************************************
	get_modify_list
********************************************************************/
void	CommitWindow::get_modify_list()
{
	GitStatus	git_status;
	FileStatusList	list	=	git_status.get_modify_files( root_path );

	emit	finish_modify_list_signal(list);
}





/*******************************************************************
	closeEvent
********************************************************************/
void	CommitWindow::closeEvent( QCloseEvent *event )
{
	deleteLater();
}

