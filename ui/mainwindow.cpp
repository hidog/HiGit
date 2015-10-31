#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cassert>
#include <iostream>

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
}



/*******************************************************************
	MainWinsdow
********************************************************************/
MainWindow::~MainWindow()
{
    delete ui;
}



/*******************************************************************
	init
********************************************************************/
void	MainWindow::init()
{}


/*******************************************************************
	set_connect
********************************************************************/
void	MainWindow::set_connect()
{
	connect(	ui->cloneButton,	SIGNAL(clicked()),	this,	SLOT(clone_slot())	);
}



/*******************************************************************
	clone_slot
********************************************************************/
void	MainWindow::clone_slot()
{
	cout << "test";
}