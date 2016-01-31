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

#include "userpwdialog.h"


/*******************************************************************
	CloneWIndow
********************************************************************/
CloneWindow::CloneWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CloneWindow),
	git_ctrl(NULL)
{
    ui->setupUi(this);

	ui->abortButton->hide();
	ui->abortButton->move(ui->cancelButton->x(), ui->cancelButton->y() );

	ui->closeButton->hide();
	ui->closeButton->setEnabled(false);
	ui->closeButton->move( ui->okButton->x(), ui->okButton->y() );


	git_ctrl	=	new GitControl(this);	
	git_ctrl->get_recursive_state_func	=	boost::bind( &CloneWindow::get_recursive_state, this );     // set bind function.
	git_ctrl->get_depth_state_func		=	boost::bind( &CloneWindow::get_depth_state, this );
	git_ctrl->get_depth_num_func		=	boost::bind( &CloneWindow::get_depth_num, this );

	user_pw_dialog	=	new	UserPwDialog(this);

	ui->srcButton->setDefaultAction( new QAction(QString("URL"),this) );
	ui->srcButton->addAction( new QAction(QString("Dir"),this) );

	set_connect();
}




/*******************************************************************
	get_depth_num
********************************************************************/
int		CloneWindow::get_depth_num()
{
	int		a	=	ui->depth_num_LEdit->text().toInt();
	return	a;
}


/*******************************************************************
	get_depth_state
********************************************************************/
bool	CloneWindow::get_depth_state()
{
	if( ui->depthCBox->checkState() == Qt::Unchecked )
		return	false;
	else if( ui->depthCBox->checkState() == Qt::Checked )
		return	true;
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
	connect(	ui->srcButton,			SIGNAL(clicked()),							this,				SLOT(src_slot())								);
	connect(	ui->destButton,			SIGNAL(clicked()),							this,				SLOT(dest_slot())								);

	connect(	ui->okButton,			SIGNAL(clicked()),							this,				SLOT(ok_slot())									);
	connect(	ui->cancelButton,		SIGNAL(clicked()),							this,				SLOT(cancel_slot())								);
	connect(	ui->closeButton,		SIGNAL(clicked()),							this,				SLOT(close_slot())								);
	connect(	ui->abortButton,		SIGNAL(clicked()),							this,				SLOT(abort_slot())								);
	connect(	ui->srcButton,			SIGNAL(triggered(QAction*)),				ui->srcButton,		SLOT(setDefaultAction(QAction*))				);

	connect(	ui->depthCBox,			SIGNAL(stateChanged(int)),					this,				SLOT(depth_slot(int))							);
	connect(	ui->depth_num_LEdit,	SIGNAL(textChanged(const QString)),			this,				SLOT(depth_num_changed_slot(const QString))		);

	//connect(	git_ctrl,				SIGNAL(output_signal(QByteArray)),			this,				SLOT(output_slot(QByteArray))					);
	connect(	git_ctrl,				SIGNAL(output_signal(QList<QByteArray>)),	this,				SLOT(output_slot(QList<QByteArray>))			);
	connect(	git_ctrl,				SIGNAL(progress_signal(int)),				ui->gitProgress,	SLOT(setValue(int))								);
	connect(	git_ctrl,				SIGNAL(need_user_pw_signal()),				this,				SLOT(need_user_pw_slot())						);
	connect(	git_ctrl,				SIGNAL(cmd_finished_signal()),				this,				SLOT(git_clone_finished_slot())					);

	connect(	user_pw_dialog,			SIGNAL(userpw_signal(QString,QString)),		this,				SLOT(userpw_slot(QString,QString))				);
}



/*******************************************************************
	depth_num_changed_slot
********************************************************************/
void	CloneWindow::depth_num_changed_slot( const QString str )
{
	QString		tmp		=	str;

	int		i;

	// remove all non-num
	for( i = 0; i < tmp.length(); i++ )
	{
		if( tmp[i] < '0' || tmp[i] > '9' )
		{
			tmp.remove( i, 1 );
			i--;
		}
	}

	if( tmp.isEmpty() )
		tmp		=	"0";

	ui->depth_num_LEdit->setText(tmp);
}




/*******************************************************************
	depth_slot
********************************************************************/
void	CloneWindow::depth_slot( int state )
{
	if( state == Qt::Unchecked )
		ui->depth_num_LEdit->setEnabled(false);
	else if( state == Qt::Checked )
		ui->depth_num_LEdit->setEnabled(true);
}


/*******************************************************************
	git_clone_finished_slot
********************************************************************/
void	CloneWindow::git_clone_finished_slot()
{
	ui->abortButton->setEnabled(false);
	ui->closeButton->setEnabled(true);
}



/*******************************************************************
	close_slot
********************************************************************/
void	CloneWindow::close_slot()
{
	this->close();
}



/*******************************************************************
	abort_slot
********************************************************************/
void	CloneWindow::abort_slot()
{
	git_ctrl->abort_cmd();
	ui->abortButton->setEnabled(false);
}



/*******************************************************************
	need_user_pw_slot
********************************************************************/
void	CloneWindow::need_user_pw_slot()
{
	//printf("test");
	user_pw_dialog->show();
}



/*******************************************************************
	need_user_pw_slot
********************************************************************/
void	CloneWindow::userpw_slot( QString username, QString password )
{
	QString		src		=	ui->srcLEdit->text();
	QString		dest	=	ui->destLEdit->text();

	git_ctrl->clone( src, dest, username, password );

	ui->abortButton->setEnabled(true);
	ui->closeButton->setEnabled(false);
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
	QRegExp		rexp("(\\S)");

	if( rexp.indexIn(src) == -1 || rexp.indexIn(dest) == -1 )
		return;

	git_ctrl->clone( src, dest );

	ui->okButton->hide();
	ui->cancelButton->hide();
	ui->closeButton->show();
	ui->abortButton->show();
}




/*******************************************************************
	cancel_slot
********************************************************************/
void	CloneWindow::cancel_slot()
{
	this->close();
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
		// http://www.dwheeler.com/essays/open-files-urls.html
#ifdef _WIN32
		if( ui->srcLEdit->text().length() > 0 )
		{
			char	str[1000];
			sprintf( str, "start %s", ui->srcLEdit->text().toStdString().c_str() );
			system(str);
		}
#else
		QMessageBox::critical(this,"warning", "nee maintain." );
		//open filename_or_URL
		//xdg-open filename_or_URL
#endif
	}
}




/*******************************************************************
	dest_slot
********************************************************************/
void	CloneWindow::dest_slot()
{
	QString		path	=	QFileDialog::getExistingDirectory( NULL, tr("Choose destination"), ui->destLEdit->text() );

	if( path.isEmpty() == false )
		ui->destLEdit->setText(path);
}