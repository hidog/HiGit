#include "commitwindow.h"
#include "ui_commitwindow.h"

#include "../src/git_cmd/git_status.h"
#include "../src/tools.h"
#include "../src/git_control.h"

#include <QDebug>
#include <QCheckBox>
#include <QDir>
#include <QPushButton>

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

    ui->buttonBox->button( QDialogButtonBox::Ok )->setEnabled(false);
	ui->fileTWidget->setColumnWidth( 0, 20 );
	ui->fileTWidget->setColumnWidth( 1, 300 );

	connect(	this,           SIGNAL(finish_modify_list_signal(QList<FileStatus>)),		this,		SLOT(finish_modify_list_slot(QList<FileStatus>))	);
    connect(    ui->textEdit,   SIGNAL(textChanged()),                                      this,       SLOT(text_changed_slot())                           );
    connect(    ui->buttonBox,  SIGNAL(accepted()),                                         this,       SLOT(accepted_slot())                               );

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
	accepted_slot
 ********************************************************************/
void    CommitWindow::accepted_slot()
{
    // get file list
    QStringList     list    =   get_commit_list();
    
    // add
    //GitControl  git_control;
    //git_control->add( root_path, list );
    
    //foreach( QString str, list )
    //    qDebug() << str;
}



/*******************************************************************
	get_commit_list
********************************************************************/
QStringList     CommitWindow::get_commit_list()
{
    int     i;
    int     size    =   ui->fileTWidget->rowCount();
    
    QCheckBox       *cbox   =   NULL;
    QStringList     list;
    QTableWidget    *table  =   ui->fileTWidget;
    
    for( i = 0; i < size; i++ )
    {
        cbox    =   static_cast<QCheckBox*>( ui->fileTWidget->cellWidget( i, 0 ) );
        if( cbox->checkState() == Qt::Checked )
            list.push_back( table->item(i,1)->text() );
    }
    
    return  list;
}


/*******************************************************************
	text_changed_slot
********************************************************************/
void    CommitWindow::text_changed_slot()
{
    if( ui->textEdit->toPlainText().length() > 0 ) 
        ui->buttonBox->button( QDialogButtonBox::Ok )->setEnabled(true);
    else
        ui->buttonBox->button( QDialogButtonBox::Ok )->setEnabled(false);
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

