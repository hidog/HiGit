#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cassert>
#include <iostream>

#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QPushButton>

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
    
    // load db data to init.
    init_proj_button();
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
	init_proj_button
 ********************************************************************/
void    MainWindow::init_proj_button()
{
    bcListDbProj    list    =   db_mng->get_all_proj();
    string         path;
    
    for( boost::container::list<DbProj>::iterator itr = list.begin(); itr != list.end(); ++itr )
    {
        cout << itr->path << "\n" << itr->name << "\n\n";
        path    =   itr->path;
    }
    
   // QWidget     *widget     =   new QWidget();
   // widget->resize(1000,1000);
    
    //ui->scrollArea->setWidget(widget);
    
    //QVBoxLayout *layout = new QVBoxLayout(ui->scrollArea);
    ui->scrollArea->widget()->setMinimumHeight( 1000*30 );
    
    for( int i = 0; i < 1000; i++ )
    {
        QPushButton     *ptr    =   new QPushButton( QString("%1 %2").arg(path.c_str()).arg(i), ui->scrollArea->widget() );
        ptr->move( 0, i*30 );
    }
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

    if( path.size() > 0 )
    {
    
        QString		name	=	git_ctrl->get_proj_name(path);
        db_mng->add_proj( path.toStdString(), name.toStdString() );
    }
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