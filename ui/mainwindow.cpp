#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cassert>
#include <iostream>

#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>

#include "../ui/clonewindow.h"
#include "../src/git_control.h"
#include "../src/db_manager.h"

using namespace std;

/*******************************************************************
	MainWindow
********************************************************************/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	init();
	set_connect();

	db_mng	=	new DbManager();

	if( git_ctrl->check_git_exist() == false )
	{
		// choose git exe file.
	}
}






/*******************************************************************
	MainWinsdow
********************************************************************/
MainWindow::~MainWindow()
{
	delete git_ctrl;	git_ctrl	=	NULL;

    delete ui;
}



/*******************************************************************
	init
********************************************************************/
void	MainWindow::init()
{
	git_ctrl	=	new GitControl;
}


/*******************************************************************
	set_connect
********************************************************************/
void	MainWindow::set_connect()
{
	connect(	ui->cloneButton,	SIGNAL(clicked()),	this,	SLOT(clone_slot())	);
	connect(	ui->initButton,		SIGNAL(clicked()),	this,	SLOT(init_slot())	);
	connect(	ui->openButton,		SIGNAL(clicked()),	this,	SLOT(open_slot())	);
}


/*******************************************************************
	init_slot
********************************************************************/
void	MainWindow::init_slot()
{
	QString		path	=	QFileDialog::getExistingDirectory();
	QString		name	=	git_ctrl->get_proj_name(path);	
	
	if( git_ctrl->init( path ) == true )
		db_mng->add_proj( path.toStdString(), name.toStdString() );
}




/*******************************************************************
	open_slot
********************************************************************/
void	MainWindow::open_slot()
{
	QString		path		=	QFileDialog::getExistingDirectory();
	QString		root_path	=	git_ctrl->check_exist_git_repository(path);


	/*QString		name	=	git_ctrl->get_proj_name(path);	
	
	if( git_ctrl->init( path ) == true )
		db_mng->add_proj( path.toStdString(), name.toStdString() );*/
}



/*******************************************************************
	clone_slot
********************************************************************/
void	MainWindow::clone_slot()
{
	CloneWindow		*clone_window	=	new CloneWindow( this );
	connect(	clone_window,	SIGNAL(close()),	clone_window,	SLOT(deleteLater())		);
	clone_window->show();
}