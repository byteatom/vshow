#-------------------------------------------------
#
# Project created by QtCreator 2014-12-29T22:34:12
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qlog
TEMPLATE = app

CONFIG -= debug_and_release

CONFIG(debug, debug|release) {
	BUILD_CFG=debug
} else {
	BUILD_CFG=release
}

DESTDIR = $$_PRO_FILE_PWD_/../run/$$TARGET-$${BUILD_CFG}

INCLUDEPATH +=	$$_PRO_FILE_PWD_/

LIBS += -lWinmm

SOURCES += qlog/main.cpp\
	qlog/qlog.cpp \
	qlog/sender.cpp \
	qlog/var_item.cpp \
	qlog/main_wnd.cpp \
	qlog/data_view.cpp \
	util/hi_timer.cpp

HEADERS  += \
	qlog/qlog.h \
	qlog/sender.h \
	qlog/var_item.h \
	qlog/main_wnd.h \
	qlog/data_view.h \
	util/hi_timer.h

FORMS    += \
	qlog/main_wnd.ui \
	qlog/data_view.ui
