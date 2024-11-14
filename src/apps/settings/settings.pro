QT += core gui widgets sql printsupport
CONFIG += C++17
TEMPLATE = app
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
SOURCES += \
    initialsettingsdialog.cpp \
    main.cpp

HEADERS  += \
    initialsettingsdialog.h

FORMS    += \
    initialsettingsdialog.ui

OTHER_FILES += \
    settings.rc \
    settings.ico

DESTDIR = $$PWD/../../../deploy
TARGET = settings
RC_FILE = settings.rc

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

TARGET = simple-pos-settings
DESTDIR = ../../../deploy

win32:CONFIG(release, debug|release): LIBS += -L../../../deploy/ -lsimplepos-core
else:win32:CONFIG(debug, debug|release): LIBS += -L../../../deploy/ -lsimplepos-core
else:unix: LIBS += -L../../../deploy/ -lsimplepos-core

INCLUDEPATH += $$PWD/../../libs/core
DEPENDPATH += $$PWD/../../libs/core
