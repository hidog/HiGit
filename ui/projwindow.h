#ifndef PROJWINDOW_H
#define PROJWINDOW_H

#include <QMainWindow>

namespace Ui {
class ProjWindow;
}

class ProjWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProjWindow(QWidget *parent = 0);
    ~ProjWindow();

private:
    Ui::ProjWindow *ui;
};

#endif // PROJWINDOW_H
