#ifndef COMMITWINDOW_H
#define COMMITWINDOW_H

#include <QWidget>


// ~~~~~~~~~~~~~~~~~~~~~~~~~ class declare ~~~~~~~~~~~~~~~~~~~~~~~~~
namespace Ui {
class CommitWindow;
}	// end namespace Ui





/*******************************************************************
	CommitWindow
********************************************************************/
class CommitWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CommitWindow( QString _root_path, QWidget *parent = 0 );
    ~CommitWindow();

	void	closeEvent( QCloseEvent *event );
	void	get_modify_list();

private:
    Ui::CommitWindow	*ui;

	QString		root_path;
};

#endif // COMMITWINDOW_H
