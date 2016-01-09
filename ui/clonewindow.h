#ifndef CLONEWINDOW_H
#define CLONEWINDOW_H

#include <QMainWindow>


// ~~~~~~~~~~~~~~~~~~~~~~~~~ class declare ~~~~~~~~~~~~~~~~~~~~~~~~~
namespace Ui {
class CloneWindow;
}

class	GitControl;
class	UserPwDialog;



/*******************************************************************
	CloneWIndow
********************************************************************/
class CloneWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CloneWindow( QWidget *parent = 0 );
    ~CloneWindow();

	bool	get_recursive_state();

protected:
	void	closeEvent( QCloseEvent *event );



protected slots:
	void	src_slot();
	void	dest_slot();
	void	ok_slot();
	void	cancel_slot();

	void	need_user_pw_slot();
	void	userpw_slot( QString, QString );

	void	output_slot(QByteArray);
	void	output_slot(QList<QByteArray>);

private:
    Ui::CloneWindow		*ui;
	UserPwDialog		*user_pw_dialog;
	GitControl			*git_ctrl;

	void	set_connect();

};

#endif // CLONEWINDOW_H
