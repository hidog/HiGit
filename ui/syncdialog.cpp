#include "syncdialog.h"
#include "ui_syncdialog.h"

#include "../src/git_cmd/git_branch.h"


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
}



/*******************************************************************
	SyncDialog
********************************************************************/
SyncDialog::~SyncDialog()
{
    delete	ui;
}




/*******************************************************************
	init_remote_URL
********************************************************************/
void	SyncDialog::init_remote_URL()
{
	GitRemote	git_remote( root_path, this );
	QString		remote;	

	remote_list		=	git_remote.get_remote_list();

	foreach( RemoteInfo info, remote_list )
	{
		remote	=	QString("%1   <%2>").arg(info.name).arg(info.url);
		ui->remoteComboBox->addItem(remote);
	}
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
