#-------------------------------------------------
#
# Project created by QtCreator 2015-10-31T22:03:11
#
#-------------------------------------------------

message("HiGit...")

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win32 {
CONFIG += console
CONFIG -= app_bundle
}

TARGET		=	HiGit
TEMPLATE	=	app

INCLUDEPATH 	+=	.

win32 {
	message("win32")
	Release:DESTDIR	=	../release
	Debug:DESTDIR	=	../debug
	INCLUDEPATH		+=	C:\Boost\include\boost-1_60
	LIBPATH			+=	C:\Boost\lib
}

macx {
	message("macx")
	INCLUDEPATH 	+=	/usr/local/include
	LIBPATH 		+=	/usr/local/lib
	OBJECTS_DIR 	=	./obj
	MOC_DIR 		=	./obj
	RCC_DIR 		=	./obj
	UI_DIR 			=	./obj
}

SOURCES		+=	main.cpp\
				ui/mainwindow.cpp\
				ui/clonewindow.cpp\
				ui/userpwdialog.cpp\
				ui/projectbutton.cpp\
				src/git_control.cpp\
				src/db_manager.cpp\
				src/sqlite3.c\
				src/git_cmd/git_command.cpp\
				src/git_cmd/git_clone.cpp
    

HEADERS		+=	ui/mainwindow.h\
				ui/clonewindow.h\
				ui/userpwdialog.h\
				ui/projectbutton.h\
				src/git_control.h\
				src/db_manager.h\
				src/sqlite3.h\
				src/git_cmd/git_command.h\
				src/git_cmd/git_clone.h\
				src/def.h
				

FORMS    	+= 	ui/mainwindow.ui\
				ui/clonewindow.ui\
				ui/userpwdialog.ui\
				ui/projectbutton.ui
