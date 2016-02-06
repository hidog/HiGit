#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>


// ~~~~~~~~~~~~~~~~~~~~~~~~~ class declare ~~~~~~~~~~~~~~~~~~~~~~~~~
namespace Ui {
	class MainWindow;
}

class GitControl;
class DbManager;


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
    
public slots:

protected slots:
	void	clone_slot();
	void	init_slot();
	void	open_slot();

private:
	GitControl	*git_ctrl;

	void	init();
	void	set_connect();

    Ui::MainWindow *ui;

	DbManager	*db_mng;
};

#endif // MAINWINDOW_H
