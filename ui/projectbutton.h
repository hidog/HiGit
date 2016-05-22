#ifndef PROJECTBUTTON_H
#define PROJECTBUTTON_H


#include <QPushButton>
#include "../src/db_manager.h"
#include "../src/def.h"


#define	PJ_BUTTON_WIDTH			370
#define PJ_BUTTON_HEIGHT		80


// ~~~~~~~~~~~~~~~~~~~~~ class declare ~~~~~~~~~~~~~~~~~~~~~
namespace Ui {
class ProjectButton;
} // end namespace Ui

class ProjWindow;


// ~~~~~~~~~~~~~~~~~~~~~ enum ~~~~~~~~~~~~~~~~~~~~~
namespace pjb {
enum	ERR_STATE
{
	NO_ERROR,
	FOLDER_NOT_EXIST,
	GIT_REPO_NOT_EXIST,
};
} // end namepace projectbutton




/*******************************************************************
	ProjectButton
********************************************************************/
class ProjectButton : public QPushButton
{
    Q_OBJECT

public:
    explicit ProjectButton( DbProj _proj, QWidget *parent = 0, QWidget *main_window = 0 );
    ~ProjectButton();

	void		set_error( pjb::ERR_STATE es );
	QString		get_path();

	static int	fixed_width();
	static int	fixed_height();

	virtual void	mouseDoubleClickEvent( QMouseEvent *event );

public slots:

	void	pj_window_destroyed_slot();
    void    del_project_slot();


signals:
    
    void    delete_signal( DbProj );    

private:
    Ui::ProjectButton *ui;

	pjb::ERR_STATE	err_state;

	DbProj			proj;
	ProjWindow		*pj_window;
};

#endif // PROJECTBUTTON_H
