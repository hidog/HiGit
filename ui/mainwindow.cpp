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
#include "../ui/projectbutton.h"

#ifndef Q_MOC_RUN
	#include <boost/foreach.hpp>
#endif


using namespace std;

/*******************************************************************
	MainWindow
********************************************************************/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	this->setWindowTitle(QString("HiGit"));

	proj_list.clear();

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
	resizeEvent
********************************************************************/
void	MainWindow::resizeEvent( QResizeEvent *event )
{
	//cout << ui->scrollArea->width() << " " << ui->scrollArea->height() << endl;

	int		width			=	ui->scrollArea->width();

	ProjectButton	*button;	

	foreach( button, proj_list )
		button->setFixedWidth( width );
}




/*******************************************************************
	delete_slot
********************************************************************/
void    MainWindow::delete_slot( DbProj proj )
{
    //QMessageBox::about( 0, "test", "test");
    db_mng->delete_proj( proj );
    
    remove_all_proj_button();
    init_proj_button();
}


/*******************************************************************
	remove_all_proj_button
********************************************************************/
void    MainWindow::remove_all_proj_button()
{
    ProjectButton   *button;
    
    foreach( button, proj_list )
        delete  button;

    proj_list.clear();
}



/*******************************************************************
	check_folder_exist
********************************************************************/
bool	MainWindow::check_folder_exist( std::string path )
{
	QDir	dir( path.c_str() );
	return	dir.exists();
}



/*******************************************************************
	init_proj_button
********************************************************************/
void    MainWindow::init_proj_button()
{
    bcListDbProj    list    =   db_mng->get_all_proj();

	int		count	=	0;
	int		width	=	ui->scrollArea->width();
	
	BOOST_FOREACH( DbProj proj, list )
	{
		// add button
		proj_list.push_back( new ProjectButton( proj, ui->scrollArea->widget(), this ) );
		proj_list.last()->move( 0, count*ProjectButton::fixed_height() );
        proj_list.last()->setVisible(true);
		proj_list.last()->setFixedWidth( width );
		count++;
		// 
		if( check_folder_exist( proj.path ) == false )
			proj_list.last()->set_error( pjb::FOLDER_NOT_EXIST );
	}

	//int		width	=	ProjectButton::fixed_width();
	int		height	=	ProjectButton::fixed_height() * proj_list.size();

	ui->scrollArea->widget()->setMinimumHeight( height );
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
	update_proj_button_slot
********************************************************************/
void	MainWindow::update_proj_button_slot( DbProj proj )
{
	if( db_mng->is_exist_proj( proj ) == false )
	{
		db_mng->add_proj( proj );
		add_ui_proj( proj );
	}
}


/*******************************************************************
	init_slot
********************************************************************/
void	MainWindow::init_slot()
{
	QString		path	=	QFileDialog::getExistingDirectory();
	QString		name	=	git_ctrl->get_proj_name(path);	
	
	if( git_ctrl->init( path ) == true )
	{
		DbProj	proj;

		proj.path		=	path.toStdString();
		proj.name		=	name.toStdString();
		proj.username	=	"";
		proj.password	=	"";

		db_mng->add_proj( proj );
		add_ui_proj( proj );
	}
}




/*******************************************************************
	open_slot
********************************************************************/
void	MainWindow::open_slot()
{
	QString		path		=	QFileDialog::getExistingDirectory();
	if( path.size() > 0 )
	{
		QString		root_path	=	git_ctrl->check_exist_git_repository(path);

		if( root_path.size() > 0 )
		{    
			QString		name	=	git_ctrl->get_proj_name(root_path);
			DbProj		proj;

			proj.name		=	name.toStdString();
			proj.path		=	root_path.toStdString();
			proj.username	=	"";
			proj.password	=	"";

			if( db_mng->is_exist_proj( proj ) == false )
			{
				// update ui, db and list.
				db_mng->add_proj( proj );
				add_ui_proj( proj );
			}
		}
        else
            QMessageBox::warning( this, "warning", "git repository not exist!");
	}
}




/*******************************************************************
	add_proj
********************************************************************/
void	MainWindow::add_ui_proj( DbProj proj )
{
	int		width	=	ui->scrollArea->width();

	proj_list.push_back( new ProjectButton( proj, ui->scrollArea->widget(), this ) );
	proj_list.last()->setVisible(true);		// needed!! otherwise view will not update.	

	int		count	=	proj_list.size();

	proj_list.last()->move( 0, (count - 1) * ProjectButton::fixed_height() );
    proj_list.last()->setMaximumWidth(10);
	proj_list.last()->setFixedWidth( width );
    
    //int		width	=	ProjectButton::fixed_width();
    int		height	=	ProjectButton::fixed_height() * proj_list.size();
    
    ui->scrollArea->widget()->setMinimumHeight( height );

}




/*******************************************************************
	clone_slot
********************************************************************/
void	MainWindow::clone_slot()
{
	CloneWindow		*clone_window	=	new CloneWindow( this );
	clone_window->show();
}
