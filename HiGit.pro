#-------------------------------------------------
#
# Project created by QtCreator 2015-10-31T22:03:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# CONFIG += console
# CONFIG -= app_bundle

TARGET		=	HiGit
TEMPLATE	=	app

Release:DESTDIR	=	../release

Debug:DESTDIR	=	../debug

INCLUDEPATH 	+=	.

macx{
INCLUDEPATH	+=	~/code/QT5/qtbase/include\
				~/code/QT5/qtbase/include/QtCore\
				~/code/QT5/qtbase/include/QtGui
}


SOURCES		+=	main.cpp\
				ui/mainwindow.cpp\
				ui/clonewindow.cpp\
				src/git_control.cpp

HEADERS		+=	ui/mainwindow.h\
				ui/clonewindow.h\
				src/git_control.h

FORMS    	+= 	ui/mainwindow.ui\
				ui/clonewindow.ui
