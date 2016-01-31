#include "projectbutton.h"
#include "ui_projectbutton.h"



/*******************************************************************
	ProjectButton
********************************************************************/
ProjectButton::ProjectButton(QWidget *parent) 
	:	QPushButton(parent),
    ui(new Ui::ProjectButton)
{
    ui->setupUi(this);
}




/*******************************************************************
	ProjectButton
********************************************************************/
ProjectButton::~ProjectButton()
{
    delete ui;
}
