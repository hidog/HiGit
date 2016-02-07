#include "projwindow.h"
#include "ui_projwindow.h"

ProjWindow::ProjWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ProjWindow)
{
    ui->setupUi(this);
}

ProjWindow::~ProjWindow()
{
    delete ui;
}
