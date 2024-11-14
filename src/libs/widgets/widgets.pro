QT += widgets

TEMPLATE = lib
DEFINES += WIDGETS_LIBRARY

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    currencyedit.cpp \
    lefttabwidget.cpp \
    numberedit.cpp \
    tabbar.cpp \
    tableview.cpp \
    tabwidget.cpp


HEADERS += \
    currencyedit.h \
    lefttabwidget.h \
    numberedit.h \
    tabbar.h \
    tableview.h \
    tabwidget.h


# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

DESTDIR = ../../../deploy
TARGET = simplepos-widgets

win32:CONFIG(release, debug|release): LIBS += -L../../../deploy/ -lsimplepos-core
else:win32:CONFIG(debug, debug|release): LIBS += -L../../../deploy/ -lsimplepos-cored
else:unix: LIBS += -L../../../deploy/ -lsimplepos-core

INCLUDEPATH += $$PWD/../core
DEPENDPATH += $$PWD/../core

FORMS += \
    lefttabwidget.ui
