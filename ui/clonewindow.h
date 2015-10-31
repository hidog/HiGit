#ifndef CLONEWINDOW_H
#define CLONEWINDOW_H

#include <QMainWindow>

namespace Ui {
class CloneWIndow;
}

class CloneWIndow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CloneWIndow(QWidget *parent = 0);
    ~CloneWIndow();

private:
    Ui::CloneWIndow *ui;
};

#endif // CLONEWINDOW_H
