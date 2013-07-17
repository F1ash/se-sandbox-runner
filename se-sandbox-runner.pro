#-------------------------------------------------
#
# Project created by QtCreator 2013-06-16T07:12:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = se-sandbox-runner
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        tray/traywidget.cpp \
        toolbar/toolbar.cpp \
        layout/listwidget.cpp \
        layout/jobmenu.cpp \
        utils/pgrep.cpp \
        element/elemprocess.cpp \
        settings/settings.cpp \
        settings/common_settings.cpp \
        settings/window_settings.cpp \
        settings/include_settings.cpp \
        settings/directory_settings.cpp \
        element/string_list.cpp \
        wait_thread.cpp

HEADERS  += mainwindow.h\
        tray/traywidget.h \
        toolbar/toolbar.h \
        layout/listwidget.h \
        layout/jobmenu.h \
        utils/pgrep.h \
        element/elemprocess.h \
        settings/settings.h \
        settings/common_settings.h \
        settings/window_settings.h \
        settings/include_settings.h \
        settings/directory_settings.h \
        element/string_list.h \
        wait_thread.h

LIBS     += -lprocps
