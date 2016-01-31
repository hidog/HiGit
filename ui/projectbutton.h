#ifndef PROJECTBUTTON_H
#define PROJECTBUTTON_H


#include <QPushButton>

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
    explicit ProjectButton(QWidget *parent = 0);
    ~ProjectButton();

private:
    Ui::ProjectButton *ui;
};

#endif // PROJECTBUTTON_H
