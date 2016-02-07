#ifndef PROJECTBUTTON_H
#define PROJECTBUTTON_H


#include <QPushButton>
#include "../src/db_manager.h"


#define	PJ_BUTTON_WIDTH			430
#define PJ_BUTTON_HEIGHT		80


namespace Ui {
class ProjectButton;
}





/*******************************************************************
	ProjectButton
********************************************************************/
class ProjectButton : public QPushButton
{
    Q_OBJECT

public:
    explicit ProjectButton( DbProj _proj, QWidget *parent = 0 );
    ~ProjectButton();

	static int	fixed_width();
	static int	fixed_height();

	virtual void	mouseDoubleClickEvent( QMouseEvent *event );

public slots:
	void	test_slot();

signals:
	void	test_sig();



private:
    Ui::ProjectButton *ui;

	DbProj	proj;
};

#endif // PROJECTBUTTON_H
