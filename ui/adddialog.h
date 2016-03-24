#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>
#include <QFileInfo>
#include <QTableWidgetItem>


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
    explicit AddDialog( QFileInfoList _add_list, QWidget *parent = 0 );
    ~AddDialog();

	void	set_add_files();

protected:
	void	closeEvent( QCloseEvent *event );


private:
    Ui::AddDialog	*ui;

	QFileInfoList	add_list;

};

#endif // ADDDIALOG_H
