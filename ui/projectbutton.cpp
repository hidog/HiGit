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
		ui(new Ui::ProjectButton)
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