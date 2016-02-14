#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QVBoxLayout>
#include "../src/def.h"


// ~~~~~~~~~~~~~~~~~~~~~~~~~ class declare ~~~~~~~~~~~~~~~~~~~~~~~~~
namespace Ui {
	class MainWindow;
}


class GitControl;
class DbManager;
class ProjectButton;


/*******************************************************************
	MainWindow
********************************************************************/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void    init_proj_button();
	void	add_ui_proj( DbProj proj );

	void	resizeEvent( QResizeEvent *event );
    
public slots:
	void	update_proj_button_slot( DbProj );


protected slots:
	void	clone_slot();
	void	init_slot();
	void	open_slot();

private:
	GitControl	*git_ctrl;

	void	init();
	void	set_connect();

    Ui::MainWindow *ui;

	DbManager		*db_mng;

	QList<ProjectButton*>	proj_list;
};

#endif // MAINWINDOW_H
