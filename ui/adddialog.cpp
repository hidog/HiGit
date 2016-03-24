#include "adddialog.h"
#include "ui_adddialog.h"




/*******************************************************************
    AddDialog
********************************************************************/
AddDialog::AddDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDialog)
{
    ui->setupUi(this);
}




/*******************************************************************
    AddDialog
********************************************************************/
AddDialog::~AddDialog()
{
    delete ui;
}
