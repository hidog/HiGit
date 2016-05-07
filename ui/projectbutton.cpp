#include "projectbutton.h"
#include "ui_projectbutton.h"

#include "projwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QString>



/*******************************************************************
	ProjectButton
********************************************************************/
ProjectButton::ProjectButton( DbProj _proj, QWidget *parent, QWidget *main_window )
	:	QPushButton(parent),
		proj(_proj),
		pj_window(NULL),
		ui(new Ui::ProjectButton),
		err_state(pjb::NO_ERROR)
{
    ui->setupUi(this);

	//this->resize( PJ_BUTTON_WIDTH, PJ_BUTTON_HEIGHT );

	ui->nameLEdit->setText( QString(proj.name.c_str()) );
	ui->pathLabel->setText( QString(proj.path.c_str()) );

    connect(    ui->delButton,      SIGNAL(clicked()),                  this,            SLOT(del_project_slot())    );
    connect(    this,               SIGNAL(delete_signal(DbProj)),      main_window,     SLOT(delete_slot(DbProj))   );
    
}




/*******************************************************************
	mouseDoubleClickEvent
********************************************************************/
void	ProjectButton::mouseDoubleClickEvent( QMouseEvent *event )
{
	if( pj_window == NULL )
	{
        pj_window   =   new ProjWindow( proj, NULL );

		connect(	pj_window,	SIGNAL(destroyed()),	this,	SLOT(pj_window_destroyed_slot())	);
		pj_window->show();		
	}
}


/*******************************************************************
	set_error
	http://stackoverflow.com/questions/21685414/qt5-setting-background-color-to-qpushbutton-and-qcheckbox
	qwidget.setStyleSheet("QPushButton:checked { background-color: red; }")
	background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,   stop:0 rgba(60, 186, 162, 255), stop:1 rgba(98, 211, 162, 255));
********************************************************************/
void	ProjectButton::set_error( pjb::ERR_STATE es )
{
	this->setStyleSheet( "background-color: rgb(255,200,200);" );

	if( err_state == pjb::NO_ERROR )
	{
		QString	err_msg		=	QString("<font color='red'>");

		err_state	=	es;
		switch( err_state )
		{
			case pjb::FOLDER_NOT_EXIST:
				err_msg	+=	QString("folder does not exist!");
				break;
			case pjb::GIT_REPO_NOT_EXIST:
				err_msg +=	QString("git repository does not exist!");
				break;
			default:
				ERRLOG("macro not defined.")
		}

		err_msg	+=	QString("</fond>");
		ui->errLabel->setText(err_msg);
	}
}



/*******************************************************************
	del_project_slot
********************************************************************/
void    ProjectButton::del_project_slot()
{
    QMessageBox::StandardButton     res;
    
    QString text    =   QString( "delete proj %1?" ).arg( proj.name.c_str() );
    
    res     =   QMessageBox::question( this, "delete", text, QMessageBox::Yes | QMessageBox::No, QMessageBox::No );
    
    if( res == QMessageBox::Yes )
    {
        //QMessageBox::about(this,"delete.","delete");
        emit delete_signal( proj );
    }
}



/*******************************************************************
	pj_window_destroyed_slot
********************************************************************/
void	ProjectButton::pj_window_destroyed_slot()
{
	pj_window	=	NULL;
}





/*******************************************************************
	ProjectButton
********************************************************************/
ProjectButton::~ProjectButton()
{
    delete ui;
}





/*******************************************************************
	fixed_width
********************************************************************/
int		ProjectButton::fixed_width()
{
	return	PJ_BUTTON_WIDTH;
}



	

/*******************************************************************
	fixed_height
********************************************************************/
int		ProjectButton::fixed_height()
{
	return	PJ_BUTTON_HEIGHT;
}