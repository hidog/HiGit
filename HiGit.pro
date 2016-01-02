#-------------------------------------------------
#
# Project created by QtCreator 2015-10-31T22:03:11
#
#-------------------------------------------------

message("test...")

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += console
CONFIG -= app_bundle

TARGET		=	HiGit
TEMPLATE	=	app

INCLUDEPATH 	+=	.

win32 
{
	Release:DESTDIR	=	../release
	Debug:DESTDIR	=	../debug
}

macx 
{
	OBJECTS_DIR 	=	./obj
	MOC_DIR 		=	./obj
	RCC_DIR 		=	./obj
	UI_DIR 			=	./obj
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
