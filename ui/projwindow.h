#ifndef PROJWINDOW_H
#define PROJWINDOW_H

#include <QMainWindow>

#include "../src/def.h"

namespace Ui {
class ProjWindow;
}




/*******************************************************************
	ProjWindow
********************************************************************/
class ProjWindow : public QMainWindow
{
    Q_OBJECT

public:
	explicit ProjWindow( DbProj _proj, QWidget *parent = 0 );
    ~ProjWindow();

	void	set_connect();
	void	set_action();


public slots:
	void	commit_slot();
	void	pull_slot();
	void	push_slot();

signals:


protected:
	void	closeEvent( QCloseEvent *event ) override;


private:
    Ui::ProjWindow *ui;

	DbProj	proj;
};

#endif // PROJWINDOW_H
