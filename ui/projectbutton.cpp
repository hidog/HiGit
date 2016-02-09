#include "projectbutton.h"
#include "ui_projectbutton.h"

#include "projwindow.h"
#include <QDebug>




/*******************************************************************
	ProjectButton
********************************************************************/
ProjectButton::ProjectButton( DbProj _proj, QWidget *parent) 
	:	QPushButton(parent),
		proj(_proj),
		pj_window(NULL),
		ui(new Ui::ProjectButton)
{
    ui->setupUi(this);

	//this->resize( PJ_BUTTON_WIDTH, PJ_BUTTON_HEIGHT );

	ui->nameLEdit->setText( QString(proj.name.c_str()) );
	ui->pathLabel->setText( QString(proj.path.c_str()) );

}




/*******************************************************************
	mouseDoubleClickEvent
********************************************************************/
void	ProjectButton::mouseDoubleClickEvent( QMouseEvent *event )
{
	if( pj_window == NULL )
	{
		pj_window		=	new ProjWindow( proj, this );
		connect(	pj_window,	SIGNAL(destroyed()),	this,	SLOT(pj_window_destroyed_slot())	);
		pj_window->show();
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