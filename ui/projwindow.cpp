#include "projwindow.h"
#include "ui_projwindow.h"

#include "commitwindow.h"



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

	connect(	ui->commitButton,	SIGNAL(clicked()),	this,	SLOT(commit_slot())		);
}




/*******************************************************************
	ProjWindow
********************************************************************/
ProjWindow::~ProjWindow()
{
    delete ui;
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
