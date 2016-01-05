#include "clonewindow.h"
#include "ui_clonewindow.h"

#include <QFileDialog>
#include <QDebug>
#include <QMenu>
#include <QMessageBox>

#ifndef Q_MOC_RUN
#	include <boost/function.hpp>
#	include<boost/bind.hpp>
#endif

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

	git_ctrl	=	new GitControl(this);
	// set bind function.
	git_ctrl->get_recursive_state_func	=	boost::bind( &CloneWindow::get_recursive_state, this );

	ui->srcButton->setDefaultAction(  new QAction(QString("Dir"),this) );
	//ui->srcButton->addAction( new QAction(QString("Dir"),this) );
	ui->srcButton->addAction( new QAction(QString("URL"),this) );

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
	connect(	ui->srcButton,			SIGNAL(clicked()),							this,				SLOT(src_slot())						);
	connect(	ui->destButton,			SIGNAL(clicked()),							this,				SLOT(dest_slot())						);
	connect(	ui->okButton,			SIGNAL(clicked()),							this,				SLOT(ok_slot())							);
	connect(	ui->cancelButton,		SIGNAL(clicked()),							this,				SLOT(cancel_slot())						);
	connect(	ui->srcButton,			SIGNAL(triggered(QAction*)),				ui->srcButton,		SLOT(setDefaultAction(QAction*))		);

	connect(	git_ctrl,				SIGNAL(output_signal(QByteArray)),			this,				SLOT(output_slot(QByteArray))			);
	connect(	git_ctrl,				SIGNAL(output_signal(QList<QByteArray>)),	this,				SLOT(output_slot(QList<QByteArray>))	);
	connect(	git_ctrl,				SIGNAL(progress_signal(int)),				ui->gitProgress,	SLOT(setValue(int))						);

	
}




/*******************************************************************
	get_recursive_state
********************************************************************/
bool	CloneWindow::get_recursive_state()
{
	if( ui->recsCBox->checkState() == Qt::Unchecked )
		return	false;
	else if( ui->recsCBox->checkState() == Qt::Checked )
		return	true;
	else
		return	false;		// Qt::PartiallyChecked

}




/*******************************************************************
	output_slot
********************************************************************/
void	CloneWindow::output_slot( QByteArray output )
{
	ui->outputTBrowser->append(output);
}



/*******************************************************************
	output_slot
********************************************************************/
void	CloneWindow::output_slot( QList<QByteArray> list )
{
	ui->outputTBrowser->clear();

	QList<QByteArray>::iterator		itr;
	for( itr = list.begin(); itr != list.end(); ++itr )
		ui->outputTBrowser->append(*itr);
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
	if( ui->srcButton->defaultAction()->text() == QString("Dir") )
	{
		QString		src		=	ui->srcLEdit->text();
		QString		path	=	QFileDialog::getExistingDirectory( NULL, "Open Directory", src );

		if( path.length() != 0 )
			ui->srcLEdit->setText(path);
	}
	else if( ui->srcButton->defaultAction()->text() == QString("URL") )
	{
		QMessageBox::critical(this,"warning", "nee maintain." );
	}
}




/*******************************************************************
	dest_slot
********************************************************************/
void	CloneWindow::dest_slot()
{
	QString		path	=	QFileDialog::getExistingDirectory();
	ui->destLEdit->setText(path);
}