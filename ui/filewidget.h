﻿#ifndef FILEWIDGET_H
#define FILEWIDGET_H

#include <QWidget>
#include <string>
#include <QDir>
#include <QMenu>

#include <boost/container/vector.hpp>

using namespace std;



namespace Ui {
	class FileWidget;
}

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

	void clicked_rightMenu(const QPoint &pos); 

signals:
	void	enter_dir_signal( const QModelIndex & );
    
private:

	boost::container::vector<int>	header_width_vec;

	void	set_connect();

    Ui::FileWidget *ui;

	FileModel	*model;
	QString		root_path;

	QMenu *rightMenu;  //右鍵菜單

};

#endif // FILEWIDGET_H
