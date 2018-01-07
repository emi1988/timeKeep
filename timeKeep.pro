#-------------------------------------------------
#
# Project created by QtCreator 2016-02-11T22:34:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = timeKeep
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    camerauye.cpp

HEADERS  += mainwindow.h \
    camerauye.h

FORMS    += mainwindow.ui

LIBS += "C:\Program Files (x86)\ZBar\lib\libzbar-0.lib" \
        "C:\opencv3.0\build\x86\vc12\lib\opencv_world300d.lib" \
        "C:\Program Files\IDS\uEye\Develop\Lib\uEye_api.lib"

INCLUDEPATH += "C:\Program Files (x86)\ZBar\include" \
               "C:\Program Files\IDS\uEye\Develop\include" \
                "C:\opencv3.0\build\include" \

DEPENDPATH += "C:\Program Files (x86)\ZBar\include" \
                "C:\Program Files\IDS\uEye\Develop\include" \
               "C:\opencv3.0\build\include" \
