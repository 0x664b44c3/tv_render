#-------------------------------------------------
#
# Project created by QtCreator 2015-05-29T00:17:13
#
#-------------------------------------------------

QT      += core gui multimedia

QMAKE_CXXFLAGS+=-std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TVRender
TEMPLATE = app



SOURCES += main.cpp\
        wndmain.cpp \
    clockrenderer.cpp \
	 nukerenderer.cpp \
	 marquee.cpp \
    fahrplan.cpp

HEADERS  += wndmain.h \
    irenderer.h \
    tvtypes.h \
    clockrenderer.h \
	 nukerenderer.h \
	 marquee.h \
    fahrplan.h
FORMS    += wndmain.ui
#unix: CONFIG += link_pkgconfig
#unix: PKGCONFIG += libavcodec libavfilter libavdevice

RESOURCES += \
    ressources.qrc
