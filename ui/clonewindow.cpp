#include "clonewindow.h"
#include "ui_clonewindow.h"

#include <QFileDialog>

#include "../src/git_control.h"




/*******************************************************************
	CloneWIndow
********************************************************************/
CloneWindow::CloneWindow(QWidget *parent, GitControl *_git_ctrl) :
    QMainWindow(parent),
    ui(new Ui::CloneWindow),
	git_ctrl(_git_ctrl)
{
    ui->setupUi(this);

	set_connect();
}




/*******************************************************************
	CloneWIndow
********************************************************************/
CloneWindow::~CloneWindow()
{
    delete ui;
}



/*******************************************************************
	set_connect
********************************************************************/
void	CloneWindow::set_connect()
{
	connect(	ui->srcButton,		SIGNAL(clicked()),	this,	SLOT(src_slot())		);
	connect(	ui->destButton,		SIGNAL(clicked()),	this,	SLOT(dest_slot())		);
	connect(	ui->okButton,		SIGNAL(clicked()),	this,	SLOT(ok_slot())			);
	connect(	ui->cancelButton,	SIGNAL(clicked()),	this,	SLOT(cancel_slot())		);


}


/*******************************************************************
	closeEvent
********************************************************************/
void	CloneWindow::closeEvent( QCloseEvent *event )
{
	deleteLater();
}



/*******************************************************************
	ok_slot
********************************************************************/
void	CloneWindow::ok_slot()
{
	QString		src		=	ui->srcLEdit->text();
	QString		dest	=	ui->destLEdit->text();

	git_ctrl->clone( src, dest );
}




/*******************************************************************
	cancel_slot
********************************************************************/
void	CloneWindow::cancel_slot()
{
}




/*******************************************************************
	src_slot
********************************************************************/
void	CloneWindow::src_slot()
{
	QString		path	=	QFileDialog::getExistingDirectory();
	ui->srcLEdit->setText(path);
}




/*******************************************************************
	dest_slot
********************************************************************/
void	CloneWindow::dest_slot()
{
	QString		path	=	QFileDialog::getExistingDirectory();
	ui->destLEdit->setText(path);
}