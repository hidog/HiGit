#ifndef SYNCDIALOG_H
#define SYNCDIALOG_H

#include <QDialog>
#include "../src/git_cmd/git_remote.h"


/*
    but: when need username/password, it will show in remote URL.
*/


// ~~~~~~~~~~~~~~~~~~~~~~~~~ class declare ~~~~~~~~~~~~~~~~~~~~~~~~~
namespace Ui {
class SyncDialog;
} // end namespace Ui

class	GitControl;



/*******************************************************************
	SyncDialog
********************************************************************/
class SyncDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SyncDialog( QString _root_path, QWidget *parent = 0 );
    ~SyncDialog();	

	void	sync_pull();

protected:
	void	closeEvent( QCloseEvent *event );

protected slots:
	void	remote_name_index_change_slot( int index );
	void	download_slot();

	void	output_slot(QList<QByteArray>);
	void	need_user_pw_slot();

private:

	void	init_local_branch();	
	void	init_remote_URL();
	void	init_remote_branch();
	void	set_connect();

    Ui::SyncDialog	*ui;
	const QString	root_path;

	RemoteInfoList	remote_list;
	GitControl		*git_ctrl;
};

#endif // SYNCDIALOG_H
