#-------------------------------------------------
#
# Project created by QtCreator 2013-03-13T15:04:10
#
#-------------------------------------------------

QT       += core gui network sql phonon

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DaoTingTuShuo
TEMPLATE = app

DEFINES *= QT_USE_QSTRINGBUILDER


SOURCES += main.cpp\
        widget.cpp \
    maptilesframe.cpp \
    dlgabout.cpp \
    dlgsetup.cpp \
    zoomeffect.cpp \
    maptilesprovider.cpp \
    routewidget.cpp \
    graphics.cpp \
    locationwidget.cpp \
    navigationimitation.cpp \
    playthread.cpp \
    soundmanager.cpp \
    dlgimgpopup.cpp

HEADERS  += widget.h \
    maptilesframe.h \
    dlgabout.h \
    dlgsetup.h \
    zoomeffect.h \
    maptilesprovider.h \
    routewidget.h \
    graphics.h \
    locationwidget.h \
    navigationimitation.h \
    playthread.h \
    soundmanager.h \
    dlgimgpopup.h

FORMS    += widget.ui \
    maptilesframe.ui \
    dlgabout.ui \
    dlgsetup.ui \
    zoomeffect.ui \
    locationwidget.ui \
    dlgimgpopup.ui

RESOURCES += \
    images.qrc

OTHER_FILES += \
    interface.rc

RC_FILE = \
    interface.rc
