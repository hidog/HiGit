#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>


// ~~~~~~~~~~~~~~~~~~~~~~~~~ class declare ~~~~~~~~~~~~~~~~~~~~~~~~~
namespace Ui {
class AddDialog;
}	// end namespace Ui





/*******************************************************************
	AddDialog
********************************************************************/
class	AddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddDialog(QWidget *parent = 0);
    ~AddDialog();

private:
    Ui::AddDialog	*ui;
};

#endif // ADDDIALOG_H
