QT       += core gui widgets
DESTDIR = ../../../deploy
TARGET = testapp
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    widget.cpp

HEADERS += \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


win32:CONFIG(release, debug|release): LIBS += -L../../../deploy/ -lsimplepos-core
else:win32:CONFIG(debug, debug|release): LIBS += -L../../../deploy/ -lsimplepos-core
else:unix: LIBS += -L../../../deploy/ -lsimplepos-core

INCLUDEPATH += $$PWD/../../libs/core
DEPENDPATH += $$PWD/../../libs/core

win32:CONFIG(release, debug|release): LIBS += -L../../../deploy/ -lsimplepos-widgets
else:win32:CONFIG(debug, debug|release): LIBS += -L../../../deploy/ -lsimplepos-widgets
else:unix: LIBS += -L../../../deploy/ -lsimplepos-widgets

INCLUDEPATH += $$PWD/../../libs/widgets
DEPENDPATH += $$PWD/../../libs/widgets
