#include "syncdialog.h"
#include "ui_syncdialog.h"

#include "../src/git_cmd/git_branch.h"
#include "../src/def.h"
#include "../src/git_control.h"


/*******************************************************************
	SyncDialog
********************************************************************/
SyncDialog::SyncDialog( QString _root_path, QWidget *parent ) :
	root_path(_root_path),
    QDialog(parent),
    ui(new Ui::SyncDialog),
	git_ctrl(NULL)
{
    ui->setupUi(this);

	git_ctrl	=	new GitControl();
	if( NULL == git_ctrl )
		ERRLOG("mem alloc fail.")

	// following has ordered property
	// get local branch
	init_local_branch();
	// get remote URL
	init_remote_URL();
	// get remote branch
	init_remote_branch();

	// set ToolButton
	ui->downloadTButton->setDefaultAction( new QAction(QString("PULL"),this) );

	//
	set_connect();
}



/*******************************************************************
	SyncDialog
********************************************************************/
SyncDialog::~SyncDialog()
{
	delete	git_ctrl;
    delete	ui;
}




/*******************************************************************
	set_connect
********************************************************************/
void	SyncDialog::set_connect()
{
	connect(	ui->rnameCBox,			SIGNAL(currentIndexChanged(int)),	this,	SLOT(remote_name_index_change_slot(int))	);
	connect(	ui->downloadTButton,	SIGNAL(clicked()),					this,	SLOT(download_slot())						);
}



/*******************************************************************
	download_slot
********************************************************************/
void	SyncDialog::download_slot()
{
	if( ui->downloadTButton->defaultAction()->text() == QString("PULL") )
		sync_pull();
	else
		ERRLOG("need maintain.")
}




/*******************************************************************
	sync_pull
********************************************************************/
void	SyncDialog::sync_pull()
{
	//QString		remote	=	ui->rnameCBox->currentText();
	//QString		branch	=	

	git_ctrl->pull( root_path );

}



/*******************************************************************
	init_remote_branch
********************************************************************/
void	SyncDialog::init_remote_branch()
{
	QString		current_remote	=	ui->rnameCBox->currentText();

	ui->rbranchCBox->clear();

	if( current_remote.isEmpty() == false )
	{
		GitBranch		git_branch( root_path, this );
		QStringList		list	=	git_branch.get_remote_branch( current_remote );
		QString			local_branch	=	ui->lbranchCBox->currentText();

		int		i,	index;
		QStringList::iterator	itr;

		// add 
		index	=	-1;
		for( itr = list.begin(), i = 0; itr != list.end(); ++itr, ++i )
		{
			if( *itr == local_branch )
				index	=	i;
			ui->rbranchCBox->addItem( *itr );
		}

		// set as current.
		if( index >= 0 )
			ui->rbranchCBox->setCurrentIndex(index);		
		else
		{
			ui->rbranchCBox->insertItem( 0, QString("") );
			ui->rbranchCBox->setCurrentIndex(0);
		}
	}
}



/*******************************************************************
	remote_name_index_change_slot
********************************************************************/
void	SyncDialog::remote_name_index_change_slot( int index )
{
	if( index >= remote_list.size() )
	{
		ERRLOG("index error")
		return;
	}

	// 
	QString		url		=	index >= 0 ? remote_list[index].url : QString("");

	ui->urlLEdit->setText(url);
}


/*******************************************************************
	init_remote_URL
********************************************************************/
void	SyncDialog::init_remote_URL()
{
	GitRemote	git_remote( root_path, this );

	remote_list		=	git_remote.get_remote_list();

	// insert remote name.
	foreach( RemoteInfo info, remote_list )
		ui->rnameCBox->addItem(info.name);

	// 
	int			index	=	ui->rnameCBox->currentIndex();
	QString		url		=	index >= 0 ? remote_list[index].url : QString("");

	ui->urlLEdit->setText(url);
}


/*******************************************************************
	local_branch
********************************************************************/
void	SyncDialog::init_local_branch()
{
	GitBranch		git_branch(root_path);
	QStringList		list	=	git_branch.get_all_branch();

	int		i,	index;
	QStringList::iterator	itr;

	//
	for( itr = list.begin(), i = 0; itr != list.end(); ++itr, ++i )
	{
		if( itr->at(0) == '*' )
		{
			index	=	i;
			itr->remove( 0, 1 );
		}
		ui->lbranchCBox->addItem( *itr );
	}

	// se as current branch 
	ui->lbranchCBox->setCurrentIndex(index);
}



/*******************************************************************
	closeEvent
********************************************************************/
void	SyncDialog::closeEvent( QCloseEvent *event )
{
	deleteLater();
}
