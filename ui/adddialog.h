#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>

namespace Ui {
class adddialog;
}

class adddialog : public QDialog
{
    Q_OBJECT

public:
    explicit adddialog(QWidget *parent = 0);
    ~adddialog();

private:
    Ui::adddialog *ui;
};

#endif // ADDDIALOG_H
