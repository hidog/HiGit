#ifndef FILEWIDGET_H
#define FILEWIDGET_H

#include <QWidget>
#include <string>
#include <QDir>

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

signals:
	void	enter_dir_signal( const QModelIndex & );
    
private:

	void	set_connect();

    Ui::FileWidget *ui;

	FileModel	*model;
	QString		root_path;
};

#endif // FILEWIDGET_H
