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



/*******************************************************************
	SyncDialog
********************************************************************/
class SyncDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SyncDialog( QString _root_path, QWidget *parent = 0 );
    ~SyncDialog();	

protected:
	void	closeEvent( QCloseEvent *event );

protected slots:
	void	remote_name_index_change_slot( int index );

private:

	void	init_local_branch();	
	void	init_remote_URL();

    Ui::SyncDialog	*ui;
	const QString	root_path;

	RemoteInfoList	remote_list;
};

#endif // SYNCDIALOG_H
