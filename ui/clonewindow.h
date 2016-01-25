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
	bool	get_depth_state();
	int		get_depth_num();

protected:
	void	closeEvent( QCloseEvent *event );

protected slots:
	void	src_slot();
	void	dest_slot();

	void	depth_slot(int);
	void	depth_num_changed_slot(const QString);

	void	ok_slot();
	void	cancel_slot();
	void	close_slot();
	void	abort_slot();

	void	need_user_pw_slot();
	void	userpw_slot( QString, QString );

	void	output_slot(QByteArray);
	void	output_slot(QList<QByteArray>);

	void	git_clone_finished_slot();

private:
    Ui::CloneWindow		*ui;
	UserPwDialog		*user_pw_dialog;
	GitControl			*git_ctrl;

	void	set_connect();

};

#endif // CLONEWINDOW_H
