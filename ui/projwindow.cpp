#include "projwindow.h"
#include "ui_projwindow.h"





/*******************************************************************
	ProjWindow
********************************************************************/
ProjWindow::ProjWindow( DbProj _proj, QWidget *parent ) :
    QMainWindow(parent),
    ui( new Ui::ProjWindow ),
	proj(_proj)
{
    ui->setupUi(this);

	this->setWindowTitle( QString(proj.name.c_str()) );

	ui->fileFW->set_root_path( QString(proj.path.c_str()) );

}




/*******************************************************************
	ProjWindow
********************************************************************/
ProjWindow::~ProjWindow()
{
    delete ui;
}






/*******************************************************************
	closeEvent
********************************************************************/
void	ProjWindow::closeEvent( QCloseEvent *event )
{
	deleteLater();
}


