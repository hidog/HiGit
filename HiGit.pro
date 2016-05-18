#-------------------------------------------------
#
# Project created by QtCreator 2015-10-31T22:03:11
#
#-------------------------------------------------

message("HiGit...")

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win32:unix:!macx {
CONFIG += console
CONFIG -= app_bundle
}

TARGET		=	HiGit
TEMPLATE	=	app

INCLUDEPATH 	+=	. ui src

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
	LIBPATH 	+=	/usr/local/lib
	OBJECTS_DIR 	=	./obj
	MOC_DIR 	=	./obj
	RCC_DIR 	=	./obj
	UI_DIR 		=	./obj
	LIBS 		+=	-lboost_thread \
				-lboost_system
	DEFINES 	+=	MACOS
}

unix:!macx{
	message("linux")
	OBJECTS_DIR 	=	./obj
	MOC_DIR 		=	./obj
	RCC_DIR 		=	./obj
	UI_DIR 			=	./obj
	LIBS 			+=	-ldl \
						-lboost_thread \
						-lboost_system
	DEFINES 		+=	LINUX
}

SOURCES		+=	main.cpp \
				ui/mainwindow.cpp \
				ui/clonewindow.cpp \
				ui/userpwdialog.cpp \
				ui/projwindow.cpp \
				ui/projectbutton.cpp \
				ui/logwidget.cpp \
				ui/filewidget.cpp \
				ui/commitwindow.cpp \
				ui/adddialog.cpp \
				ui/syncdialog.cpp \
				src/git_control.cpp \
				src/db_manager.cpp \
				src/sqlite3.c \
				src/tools.cpp \
				src/git_cmd/git_command.cpp \
				src/git_cmd/git_clone.cpp \
				src/git_cmd/git_init.cpp \
				src/git_cmd/git_status.cpp \
				src/git_cmd/git_rev_parse.cpp \
				src/git_cmd/git_base.cpp \
				src/git_cmd/git_log.cpp \
				src/git_cmd/git_commit.cpp \
				src/git_cmd/git_branch.cpp \
				src/git_cmd/git_remote.cpp \
				src/git_cmd/git_pull.cpp \
				src/git_cmd/git_push.cpp \
				model/file_model.cpp \
				model/log_model.cpp
    
    



HEADERS		+=	ui/mainwindow.h \
				ui/clonewindow.h \
				ui/userpwdialog.h \
				ui/projwindow.h \
				ui/projectbutton.h \
				ui/logwidget.h \
				ui/filewidget.h \
				ui/commitwindow.h \
				ui/adddialog.h \
				ui/syncdialog.h \
				src/git_control.h \
				src/db_manager.h \
				src/sqlite3.h \
				src/tools.h \
				src/git_cmd/git_command.h \
				src/git_cmd/git_clone.h \
				src/git_cmd/git_init.h \
				src/git_cmd/git_status.h \
				src/git_cmd/git_rev_parse.h \
				src/git_cmd/git_base.h \
				src/git_cmd/git_log.h \
				src/git_cmd/git_commit.h \
				src/git_cmd/git_branch.h \
				src/git_cmd/git_remote.h \
				src/git_cmd/git_pull.h \
				src/git_cmd/git_push.h \
				src/def.h \
				model/file_model.h \
				model/log_model.h
    
				

FORMS    	+= 	ui/mainwindow.ui \
				ui/clonewindow.ui \
				ui/userpwdialog.ui \
				ui/projwindow.ui \
				ui/projectbutton.ui \
				ui/logwidget.ui \
				ui/filewidget.ui \ 
				ui/commitwindow.ui \
				ui/adddialog.ui \
				ui/syncdialog.ui

DISTFILES +=

RESOURCES += \
    resource/mainwindow.qrc
