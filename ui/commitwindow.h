#ifndef COMMITWINDOW_H
#define COMMITWINDOW_H

#include <QDialog>
#include <QString>

#include "../src/def.h"

/*
	need make a lock view to show loading message.
*/





// ~~~~~~~~~~~~~~~~~~~~~~~~~ class declare ~~~~~~~~~~~~~~~~~~~~~~~~~
namespace Ui {
class CommitWindow;
}	// end namespace Ui

class QAbstractButton;





/*******************************************************************
	CommitWindow
********************************************************************/
class CommitWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CommitWindow( QString _root_path, QWidget *parent = 0 );
    ~CommitWindow();

	void            closeEvent( QCloseEvent *event );
	void            get_modify_list();
    
    QStringList     get_commit_list();

signals:
	void	finish_modify_list_signal( QList<FileStatus> );

protected slots:
	void	finish_modify_list_slot( QList<FileStatus> file_list );
    void    text_changed_slot();
    void    accepted_slot();
    void    button_clicked_slot( QAbstractButton* button );

private:
    void    set_connect();
    
    Ui::CommitWindow	*ui;

	QString		root_path;
};

#endif // COMMITWINDOW_H
