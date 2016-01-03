#include "clonewindow.h"
#include "ui_clonewindow.h"

#include <QFileDialog>
#include <QDebug>


#include "../src/git_control.h"




/*******************************************************************
	CloneWIndow
********************************************************************/
CloneWindow::CloneWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CloneWindow),
	git_ctrl(NULL)
{
    ui->setupUi(this);

	git_ctrl	=	new GitControl();

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
	connect(	ui->srcButton,				SIGNAL(clicked()),						this,	SLOT(src_slot())					);
	connect(	ui->destButton,				SIGNAL(clicked()),						this,	SLOT(dest_slot())					);
	connect(	ui->okButton,				SIGNAL(clicked()),						this,	SLOT(ok_slot())						);
	connect(	ui->cancelButton,			SIGNAL(clicked()),						this,	SLOT(cancel_slot())					);

	connect(	(QObject*)git_ctrl,			SIGNAL(output_signal(QByteArray)),		this,	SLOT(output_slot(QByteArray))		);
}



/*******************************************************************
	output_slot
********************************************************************/
void	CloneWindow::output_slot( QByteArray output )
{
	//qDebug() << output << "\n";
	//ui->outputTBrowser->setText(output);
	ui->outputTBrowser->append(output);
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