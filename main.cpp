#include "ui/mainwindow.h"
#include <QApplication>
#include <QMetaType>

#include "src/def.h"


int		main(int argc, char *argv[])
{
	qRegisterMetaType<DbProj>("DbProj");
	qRegisterMetaType<DbProj>("DbProj&");

	qRegisterMetaType<QVector<int>>("QVector<int>");

    QApplication	a(argc, argv);
    MainWindow		w;
    w.show();

    return a.exec();
}
