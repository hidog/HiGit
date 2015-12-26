#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cassert>
#include <iostream>

#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>

#include "../src/git_control.h"

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
}


/*******************************************************************
	init_slot
********************************************************************/
void	MainWindow::init_slot()
{
	QString		path	=	QFileDialog::getExistingDirectory();

}



/*******************************************************************
	clone_slot
********************************************************************/
void	MainWindow::clone_slot()
{
	cout << "test";
}