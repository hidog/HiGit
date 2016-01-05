#-------------------------------------------------
#
# Project created by QtCreator 2015-10-31T22:03:11
#
#-------------------------------------------------

message("HiGit...")

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += console
CONFIG -= app_bundle

TARGET		=	HiGit
TEMPLATE	=	app

INCLUDEPATH 	+=	.

win32 {
	message("win32 aaaaaa")
	Release:DESTDIR	=	../release
	Debug:DESTDIR	=	../debug
	INCLUDEPATH		+=	C:\Boost\include\boost-1_60
	LIBPATH			+=	C:\Boost\lib
}

macx {
	message("macx test")
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
				src/git_control.h\
				src/def.h

FORMS    	+= 	ui/mainwindow.ui\
				ui/clonewindow.ui
