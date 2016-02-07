#include "projectbutton.h"
#include "ui_projectbutton.h"



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