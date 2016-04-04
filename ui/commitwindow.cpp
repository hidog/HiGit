#include "commitwindow.h"
#include "ui_commitwindow.h"

#include "../src/git_cmd/git_status.h"

#include <QDebug>
#include <boost/thread.hpp>


/*******************************************************************
	CommitWIndow
********************************************************************/
CommitWindow::CommitWindow( QString _root_path, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommitWindow),
	root_path(_root_path)
{
    ui->setupUi(this);

	boost::thread	thr		=	boost::thread( boost::bind( &CommitWindow::get_modify_list, this ) );
}




/*******************************************************************
	CommitWIndow
********************************************************************/
CommitWindow::~CommitWindow()
{
    delete ui;
}




/*******************************************************************
	get_modify_list
********************************************************************/
void	CommitWindow::get_modify_list()
{
	GitStatus	git_status;
	StatusList	list	=	git_status.get_modify_files( root_path );

	for( StatusList::iterator itr = list.begin(); itr != list.end(); ++itr )
	{
		qDebug() << itr->color << " " << itr->name << " " << itr->status;
	}
}





/*******************************************************************
	closeEvent
********************************************************************/
void	CommitWindow::closeEvent( QCloseEvent *event )
{
	deleteLater();
}

