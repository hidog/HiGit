#ifndef GIT_COMMAND_H
#define GIT_COMMAND_H


#include <QObject>


/*******************************************************************
	GitCommand
********************************************************************/
class   GitCommand : public QObject
{
    Q_OBJECT

public:
    GitCommand( QWidget *parent = 0 );
    virtual ~GitCommand();

    
private:
    
};


#endif
