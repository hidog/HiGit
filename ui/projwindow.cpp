#include "projwindow.h"
#include "ui_projwindow.h"

#include "commitwindow.h"
#include "syncdialog.h"

#include "../src/git_cmd/git_branch.h"

#include <QDebug>

/*******************************************************************
	ProjWindow
********************************************************************/
ProjWindow::ProjWindow( DbProj _proj, QWidget *parent ) :
    QMainWindow(parent),
    ui( new Ui::ProjWindow ),
	proj(_proj)
{
    ui->setupUi(this);

	this->setWindowTitle( QString(proj.name.c_str()) );

	ui->fileFW->set_root_path( QString(proj.path.c_str()) );
	ui->logLW->set_root_path( QString(proj.path.c_str()) );

	// get branch name
	QString		root_path	=	proj.path.c_str();
	GitBranch	git_branch( root_path );
	QString		current_branch	=	git_branch.current_branch();
	ui->branchLEdit->setText( current_branch );

	connect(	ui->commitButton,	SIGNAL(clicked()),	this,	SLOT(commit_slot())		);
	connect(	ui->pullButton,		SIGNAL(clicked()),	this,	SLOT(pull_slot())		);
}




/*******************************************************************
	ProjWindow
********************************************************************/
ProjWindow::~ProjWindow()
{
    delete ui;
}




/*******************************************************************
	pull_slot
********************************************************************/
void	ProjWindow::pull_slot()
{
	SyncDialog	*sync_dialog	=	new SyncDialog(this);
	sync_dialog->show();
}




/*******************************************************************
	closeEvent
********************************************************************/
void	ProjWindow::closeEvent( QCloseEvent *event )
{
	deleteLater();
}




/*******************************************************************
	commit_slot
********************************************************************/
void	ProjWindow::commit_slot()
{
	QString		root_path	=	QString(proj.path.c_str());

	CommitWindow	*commit_window	=	new CommitWindow( root_path, this );
	commit_window->show();
}
