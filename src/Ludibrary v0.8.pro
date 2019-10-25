#-------------------------------------------------
#
# Project created by QtCreator 2017-07-20T19:51:11
#
#-------------------------------------------------

QT += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Ludibrary v0.8
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
        game.cpp \
    emulator.cpp \
    adddialog.cpp \
    settings.cpp \
    emulatordialog.cpp \
    searchgame.cpp \
    filedownloader.cpp \
    help.cpp \
    listedpcgame.cpp \
    listedpcgamedialog.cpp \
    dbmanager.cpp \
    editgame.cpp \
    listemulatorsdialog.cpp \
    addemulatordialog.cpp \
    editemulator.cpp \
    notification.cpp \
    notificationqueue.cpp \
    statsmonitor.cpp \
    externalsoftware.cpp \
    addexternalsoftwaredialog.cpp \
    customcheckbox.cpp \
    bubblebutton.cpp \
    bubbletext.cpp \
    listwidgetmod.cpp \
    startupwidget.cpp \
    supportedexternalsoftware.cpp

HEADERS += \
        mainwindow.h \
        game.h \
    emulator.h \
    adddialog.h \
    settings.h \
    emulatordialog.h \
    filedownloader.h \
    help.h \
    searchgame.h \
    listedpcgame.h \
    listedpcgamedialog.h \
    dbmanager.h \
    editgame.h \
    listemulatorsdialog.h \
    addemulatordialog.h \
    editemulator.h \
    notification.h \
    notificationqueue.h \
    statsmonitor.h \
    externalsoftware.h \
    addexternalsoftwaredialog.h \
    customcheckbox.h \
    bubblebutton.h \
    bubbletext.h \
    listwidgetmod.h \
    startupwidget.h \
    supportedexternalsoftware.h

FORMS += \
        game.ui \
    emulator.ui \
    settings.ui \
    mainwindow.ui \
    searchgame.ui \
    help.ui \
    listedpcgame.ui \
    listedpcgamedialog.ui \
    editgame.ui \
    listemulatorsdialog.ui \
    addemulatordialog.ui \
    editemulator.ui \
    notification.ui \
    addexternalsoftwaredialog.ui \
    startupwidget.ui

RESOURCES += \
    resource.qrc

win32: RC_ICONS = icon.ico
