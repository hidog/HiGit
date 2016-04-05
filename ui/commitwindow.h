#ifndef COMMITWINDOW_H
#define COMMITWINDOW_H

#include <QDialog>
#include "../src/def.h"

/*
	need make a lock view to show loading message.
*/





// ~~~~~~~~~~~~~~~~~~~~~~~~~ class declare ~~~~~~~~~~~~~~~~~~~~~~~~~
namespace Ui {
class CommitWindow;
}	// end namespace Ui





/*******************************************************************
	CommitWindow
********************************************************************/
class CommitWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CommitWindow( QString _root_path, QWidget *parent = 0 );
    ~CommitWindow();

	void	closeEvent( QCloseEvent *event );
	void	get_modify_list();

signals:
	void	finish_modify_list_signal( QList<FileStatus> );

protected slots:
	void	finish_modify_list_slot( QList<FileStatus> file_list );

private:
    Ui::CommitWindow	*ui;

	QString		root_path;
};

#endif // COMMITWINDOW_H
