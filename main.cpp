#include "ui/mainwindow.h"

//http://doc.qt.io/qt-4.8/qsplashscreen.html#QSplashScreen
//QSplashScreen 

#include <QApplication>
#include <QMetaType>
#include <QFileInfo>

#include "src/def.h"


int		main(int argc, char *argv[])
{
	qRegisterMetaType<DbProj>("DbProj");
	qRegisterMetaType<DbProj>("DbProj&");

    qRegisterMetaType<QVector<int> >("QVector<int>");
	qRegisterMetaType<QList<QFileInfo> >("QList<QFileInfo>");
	qRegisterMetaType<QList<FileStatus> >("QList<FileStatus>");

    QApplication	a(argc, argv);
    MainWindow		w;
    w.show();

    return a.exec();
}
