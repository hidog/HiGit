#include "userpwdialog.h"
#include "ui_userpwdialog.h"




/*******************************************************************
	UserPwDialog
********************************************************************/
UserPwDialog::UserPwDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserPwDialog)
{
    ui->setupUi(this);

	set_connect();
}



/*******************************************************************
	UserPwDialog
********************************************************************/
UserPwDialog::~UserPwDialog()
{
    delete ui;
}



/*******************************************************************
	set_connect
********************************************************************/
void	UserPwDialog::set_connect()
{
	connect(	ui->buttonBox,		SIGNAL(accepted()),		this,		SLOT(ok_slot())		);
	//connect(	ui->buttonBox,		SIGNAL(rejected()),		this,		SLOT(accept())		);
}



/*******************************************************************
	ok_slot
********************************************************************/
void	UserPwDialog::ok_slot()
{
	QString		username	=	ui->usernameLEdit->text();
	QString		password	=	ui->passwordLEdit->text();

	emit( userpw_signal(username,password) );
}
