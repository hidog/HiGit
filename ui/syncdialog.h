#ifndef SYNCDIALOG_H
#define SYNCDIALOG_H

#include <QDialog>

namespace Ui {
class SyncDialog;
}




/*******************************************************************
	SyncDialog
********************************************************************/
class SyncDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SyncDialog(QWidget *parent = 0);
    ~SyncDialog();

protected:
	void	closeEvent( QCloseEvent *event );

private:
    Ui::SyncDialog *ui;
};

#endif // SYNCDIALOG_H
