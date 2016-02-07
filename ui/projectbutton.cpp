#include "projectbutton.h"
#include "ui_projectbutton.h"
#include "projwindow.h"


/*******************************************************************
	ProjectButton
********************************************************************/
ProjectButton::ProjectButton( DbProj _proj, QWidget *parent) 
	:	QPushButton(parent),
		proj(_proj),
		ui(new Ui::ProjectButton)
{
    ui->setupUi(this);

	this->resize( PJ_BUTTON_WIDTH, PJ_BUTTON_HEIGHT );

	ui->nameLEdit->setText( QString(proj.name.c_str()) );
	ui->pathLabel->setText( QString(proj.path.c_str()) );

	connect(	this,	SIGNAL(test_sig()),	this,	SLOT(test_slot())	);
}




/*******************************************************************
	mouseDoubleClickEvent
********************************************************************/
void	ProjectButton::mouseDoubleClickEvent( QMouseEvent *event )
{
	emit test_sig();
}




/*******************************************************************
	ProjectButton
********************************************************************/
ProjectButton::~ProjectButton()
{
    delete ui;
}





/*******************************************************************
	test_slot
********************************************************************/
void	ProjectButton::test_slot()
{
	ProjWindow	*pw		=	new ProjWindow();
	pw->show();
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