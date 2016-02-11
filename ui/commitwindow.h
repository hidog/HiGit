#ifndef COMMITWINDOW_H
#define COMMITWINDOW_H

#include <QWidget>

namespace Ui {
class CommitWIndow;
}





/*******************************************************************
	CommitWIndow
********************************************************************/
class CommitWIndow : public QWidget
{
    Q_OBJECT

public:
    explicit CommitWIndow(QWidget *parent = 0);
    ~CommitWIndow();

	void	closeEvent( QCloseEvent *event );


private:
    Ui::CommitWIndow *ui;
};

#endif // COMMITWINDOW_H
