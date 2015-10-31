#include "clonewindow.h"
#include "ui_clonewindow.h"

CloneWIndow::CloneWIndow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CloneWIndow)
{
    ui->setupUi(this);
}

CloneWIndow::~CloneWIndow()
{
    delete ui;
}
