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
    explicit AddDialog( QString _root_path, QFileInfoList _add_list, QWidget *parent = 0 );
    ~AddDialog();

	void	set_add_files();
	void	set_untracked_files();

	void	set_table_widget( QTableWidget *table, const QFileInfoList &list, Qt::CheckState check_state );

protected:
	void	closeEvent( QCloseEvent *event );

protected slots:
	void	cbox_state_change_slot( int state );


private:
    Ui::AddDialog	*ui;

	QFileInfoList	add_list;
	QString			root_path;
};

#endif // ADDDIALOG_H
