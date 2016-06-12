#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QVBoxLayout>

#include "../src/def.h"

/*
	note: when close mainwindow, need close all windows. need add this feature.
*/


// ~~~~~~~~~~~~~~~~~~~~~~~~~ class declare ~~~~~~~~~~~~~~~~~~~~~~~~~
namespace Ui {
	class MainWindow;
} // end namespace Ui


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
    void    add_ui_proj( DbProj proj );
    void    remove_all_proj_button();

    void    resizeEvent( QResizeEvent *event );
	void	redraw_buttons();
    
public slots:
    void    update_proj_button_slot( DbProj );
	void	search_text_slot(const QString& text );

protected slots:
    void    clone_slot();
    void    init_slot();
    void    open_slot();
    void    delete_slot( DbProj proj );
	void	up_slot();
	void	down_slot();

private:
	GitControl	*git_ctrl;

	void	init();
	void	set_connect();

    Ui::MainWindow	*ui;
	DbManager		*db_mng;

	QList<ProjectButton*>	proj_list;
};

#endif // MAINWINDOW_H
