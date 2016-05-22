#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cassert>
#include <iostream>

#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QPushButton>
#include <QSplashScreen>
#include <QScrollBar>


#include "../ui/clonewindow.h"
#include "../src/git_control.h"
#include "../src/db_manager.h"
#include "../ui/projectbutton.h"

#include <QProgressBar>

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
		if( GitControl::check_folder_exist( proj.path ) == false )
			proj_list.last()->set_error( pjb::FOLDER_NOT_EXIST );
		else if( GitControl::check_git_repo_exist( proj.path ) == false )
			proj_list.last()->set_error( pjb::GIT_REPO_NOT_EXIST );
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
	connect(	ui->cloneButton,	SIGNAL(clicked()),						this,	SLOT(clone_slot())						);
	connect(	ui->initButton,		SIGNAL(clicked()),						this,	SLOT(init_slot())						);
	connect(	ui->openButton,		SIGNAL(clicked()),						this,	SLOT(open_slot())						);
	connect(	ui->searchLEdit,	SIGNAL(textEdited(const QString&)),		this,	SLOT(search_text_slot(const QString&))	);
}




/*******************************************************************
	search_text_slot
********************************************************************/
void	MainWindow::search_text_slot(const QString& text )
{
	ProjectButton   *btn	=	NULL;
	bool	flag;

	foreach( btn, proj_list )
	{
		flag	=	btn->get_path().contains( text, Qt::CaseInsensitive );
		btn->setVisible(flag);
	}

	redraw_buttons();
}


/*******************************************************************
	up_slot
********************************************************************/
void	MainWindow::up_slot()
{
	ProjectButton*	btn		=	(ProjectButton*)sender()->parent();
	QPoint	btn_pt	=	btn->pos();

	// find the upper btn
	bool	is_find		=	false;
	QPoint	pt;
	QList<ProjectButton*>::iterator		itr;
	for( itr = proj_list.begin(); itr != proj_list.end(); ++itr )
	{
		pt	=	(*itr)->pos();
		if( btn_pt.y() - pt.y() == ProjectButton::fixed_height() )
		{
			is_find		=	true;
			break;
		}
	}

	// move button.
	int		value;
	if( is_find )
	{
		(*itr)->move( btn_pt.x(), btn_pt.y() );
		btn->move( pt.x(), pt.y() );

		// move scroll bar.
		/*value	=	ui->scrollArea->verticalScrollBar()->value();
		if( value > ProjectButton::fixed_height() )
			ui->scrollArea->verticalScrollBar->set*/

		// update db order_value
	}
}


/*******************************************************************
	down_slot
********************************************************************/
void	MainWindow::down_slot()
{
	ProjectButton*	btn		=	(ProjectButton*)sender()->parent();
	QPoint	btn_pt	=	btn->pos();

	// find the upper btn
	bool	is_find		=	false;
	QPoint	pt;
	QList<ProjectButton*>::iterator		itr;
	for( itr = proj_list.begin(); itr != proj_list.end(); ++itr )
	{
		pt	=	(*itr)->pos();
		if( pt.y() - btn_pt.y() == ProjectButton::fixed_height() )
		{
			is_find		=	true;
			break;
		}
	}

	// move button.
	int		value;
	if( is_find )
	{
		(*itr)->move( btn_pt.x(), btn_pt.y() );
		btn->move( pt.x(), pt.y() );

		// move scroll bar.
		/*value	=	ui->scrollArea->verticalScrollBar()->value();
		if( value > ProjectButton::fixed_height() )
			ui->scrollArea->verticalScrollBar->set*/

		// update db order_value
	}
}




/*******************************************************************
	redraw_buttons
********************************************************************/
void	MainWindow::redraw_buttons()
{
	ProjectButton	*btn	=	NULL;
	int		count	=	0;
	int		height	=	ProjectButton::fixed_height();

	foreach( btn , proj_list )
	{
		if( btn->isVisible() == true )
			btn->move( 0, count++ * height );	
	}
}



/*******************************************************************
	update_proj_button_slot
********************************************************************/
void	MainWindow::update_proj_button_slot( DbProj proj )
{
	int		count;

	if( db_mng->is_exist_proj( proj ) == false )
	{
		count	=	db_mng->proj_count();
		db_mng->add_proj( proj, count+1 );
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
	int			count;

	if( git_ctrl->init( path ) == true )
	{
		DbProj	proj;

		proj.path		=	path.toStdString();
		proj.name		=	name.toStdString();
		proj.username	=	"";
		proj.password	=	"";

		count	=	db_mng->proj_count();
		db_mng->add_proj( proj, count+1 );
		add_ui_proj( proj );
	}
}




/*******************************************************************
	open_slot
********************************************************************/
void	MainWindow::open_slot()
{
	int			count;
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
				count	=	db_mng->proj_count();
				db_mng->add_proj( proj, count+1 );
				add_ui_proj( proj );
			}
		}
        else
            QMessageBox::warning( this, "warning", "git repository not exist!");
	}
}




/*******************************************************************
	add_ui_proj
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
