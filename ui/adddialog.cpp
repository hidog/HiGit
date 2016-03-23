#include "adddialog.h"
#include "ui_adddialog.h"

adddialog::adddialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::adddialog)
{
    ui->setupUi(this);
}

adddialog::~adddialog()
{
    delete ui;
}
