#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>


// ~~~~~~~~~~~~~~~~~~~~~~~~~ class declare ~~~~~~~~~~~~~~~~~~~~~~~~~
namespace Ui {
	class MainWindow;
}



/*******************************************************************
	MainWindow
********************************************************************/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:

protected slots:
	void	clone_slot();

private:
	void	init();
	void	set_connect();

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
