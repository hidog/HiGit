#ifndef USERPWDIALOG_H
#define USERPWDIALOG_H

#include <QDialog>



// ~~~~~~~~~~~~~~~~~~~~~~~~~ class declare ~~~~~~~~~~~~~~~~~~~~~~~~~
namespace Ui {
class UserPwDialog;
}




/*******************************************************************
	UserPwDialog
********************************************************************/
class UserPwDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserPwDialog(QWidget *parent = 0);
    ~UserPwDialog();

protected slots:
	void	ok_slot();

signals:
	void	userpw_signal(QString,QString);

private:
    Ui::UserPwDialog *ui;

	void	set_connect();
	
};

#endif // USERPWDIALOG_H
