#ifndef FILEWIDGET_H
#define FILEWIDGET_H

#include <QWidget>
#include <string>
#include <QDir>
#include <QMenu>

#include <boost/container/vector.hpp>

using namespace std;


/*
    bug: when delete file in folder, it will not refresh to HiGit UI correctly.
    bug: when file status is AD  (add, but delete actually). commit will show as "ADD"
 */


// ~~~~~~~~~~~~~~~~~~~~~~~~~ class declare ~~~~~~~~~~~~~~~~~~~~~~~~~
namespace Ui {
	class FileWidget;
} // end namespace Ui

class	FileModel;




/*******************************************************************
	FileWidget
********************************************************************/
class FileWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FileWidget(QWidget *parent = 0);
    ~FileWidget();

	void	set_root_path( QString _path );


public slots:
	void	double_clicked_slot( const QModelIndex &index );
	void	refresh_view_slot();
	void	header_resize_slot( int index, int old_size, int new_size );
	void	path_change_slot( QString path );
    void    add_slot();

	void	right_menu_slot( const QPoint &pos ); 

signals:
	void	enter_dir_signal( const QModelIndex & );

    
private:

	void	set_right_menu();

	boost::container::vector<int>	header_width_vec;

	void	set_connect();

    Ui::FileWidget *ui;

	FileModel	*model;
	QString		root_path;

	QMenu		*right_menu;  // mouse right click menu.
	QAction		*add_action; 

};

#endif // FILEWIDGET_H
