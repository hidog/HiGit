#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>
#include <QFileInfo>
#include <QTableWidgetItem>


// ~~~~~~~~~~~~~~~~~~~~~~~~~ class declare ~~~~~~~~~~~~~~~~~~~~~~~~~
namespace Ui {
class AddDialog;
}	// end namespace Ui

class	QCheckBox;



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
	int		search_checkbox( QCheckBox *cbox, QTableWidget *table );
	void	insert_table_item( int row, QTableWidget *table );


protected:
	void	closeEvent( QCloseEvent *event );

signals:
	void	finish_untracked_files_signal( QList<QFileInfo> );

protected slots:
	void	cbox_state_change_slot( int state );
	void	finish_untracked_files_slot( QList<QFileInfo> list );
	void	accepted_slot();

private:
    Ui::AddDialog	*ui;

	QFileInfoList	add_list;
	QString			root_path;

	bool	is_closed;
};

#endif // ADDDIALOG_H
