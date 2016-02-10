#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include <QWidget>


namespace Ui {
	class LogWidget;
}


class LogModel;



/*******************************************************************
	LogWidget
********************************************************************/
class LogWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LogWidget(QWidget *parent = 0);
    ~LogWidget();

	void	set_root_path( QString path );



private:
    Ui::LogWidget *ui;

	QString		root_path;

	LogModel	*model;
};

#endif // LOGWIDGET_H
