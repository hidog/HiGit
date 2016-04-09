#include "syncdialog.h"
#include "ui_syncdialog.h"

#include "../src/git_cmd/git_branch.h"
#include "../src/def.h"

/*******************************************************************
	SyncDialog
********************************************************************/
SyncDialog::SyncDialog( QString _root_path, QWidget *parent ) :
	root_path(_root_path),
    QDialog(parent),
    ui(new Ui::SyncDialog)
{
    ui->setupUi(this);

	// get local branch
	init_local_branch();

	// get remote URL
	init_remote_URL();

	// get remote branch

	//
	connect(	ui->remote_nameComboBox,	SIGNAL(currentIndexChanged(int)),	this,	SLOT(remote_name_index_change_slot(int))	);
}



/*******************************************************************
	SyncDialog
********************************************************************/
SyncDialog::~SyncDialog()
{
    delete	ui;
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
		ui->remote_nameComboBox->addItem(info.name);

	// 
	int			index	=	ui->remote_nameComboBox->currentIndex();
	QString		url		=	index >= 0 ? remote_list[index].url : QString("");

	ui->urlLEdit->setText(url);
}


/*******************************************************************
	local_branch
********************************************************************/
void	SyncDialog::init_local_branch()
{
	GitBranch		git_branch(root_path);
	QStringList		branch_list		=	git_branch.get_all_branch();

	foreach( QString branch, branch_list )
		ui->branchComboBox->addItem( branch );
}



/*******************************************************************
	closeEvent
********************************************************************/
void	SyncDialog::closeEvent( QCloseEvent *event )
{
	deleteLater();
}
