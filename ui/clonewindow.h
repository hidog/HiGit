#ifndef CLONEWINDOW_H
#define CLONEWINDOW_H

#include <QMainWindow>


// ~~~~~~~~~~~~~~~~~~~~~~~~~ class declare ~~~~~~~~~~~~~~~~~~~~~~~~~
namespace Ui {
class CloneWindow;
}

class	GitControl;



/*******************************************************************
	CloneWIndow
********************************************************************/
class CloneWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CloneWindow( QWidget *parent = 0, GitControl* _git_ctrl = 0 );
    ~CloneWindow();

protected:
	void	closeEvent( QCloseEvent *event );



protected slots:
	void	src_slot();
	void	dest_slot();
	void	ok_slot();
	void	cancel_slot();


private:
    Ui::CloneWindow *ui;
	GitControl	*git_ctrl;

	void	set_connect();

};

#endif // CLONEWINDOW_H
