QT += core gui widgets sql printsupport
CONFIG += c++17
RC_FILE = app.rc
SOURCES += \
    aboutdialog.cpp \
    calculatordialog.cpp \
    common.cpp \
    dashboard.cpp \
    datechooserdialog.cpp \
    db.cpp \
    libs/currencyedit.cpp \
    libs/numberedit.cpp \
    libs/tableview.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    productcategoryeditor.cpp \
    productcategorymanager.cpp \
    productcategorymodel.cpp \
    producteditor.cpp \
    productmanager.cpp \
    productmodel.cpp \
    productproxymodel.cpp \
    purchaseordereditor.cpp \
    purchaseordereditormodel.cpp \
    purchaseorderitemeditor.cpp \
    purchaseordermanager.cpp \
    purchaseordermodel.cpp \
    purchaseorderproxymodel.cpp \
    reportdatedialog.cpp \
    salesordereditor.cpp \
    salesordereditormodel.cpp \
    salesorderitemeditor.cpp \
    salesordermanager.cpp \
    salesordermodel.cpp \
    salesorderproxymodel.cpp \
    settingsdialog.cpp

HEADERS += \
    aboutdialog.h \
    calculatordialog.h \
    common.h \
    dashboard.h \
    datechooserdialog.h \
    db.h \
    global.h \
    libs/currencyedit.h \
    libs/numberedit.h \
    libs/tableview.h \
    logindialog.h \
    mainwindow.h \
    productcategoryeditor.h \
    productcategorymanager.h \
    productcategorymodel.h \
    producteditor.h \
    productmanager.h \
    productmodel.h \
    productproxymodel.h \
    purchaseordereditor.h \
    purchaseordereditormodel.h \
    purchaseorderitemeditor.h \
    purchaseordermanager.h \
    purchaseordermodel.h \
    purchaseorderproxymodel.h \
    reportdatedialog.h \
    salesordereditor.h \
    salesordereditormodel.h \
    salesorderitemeditor.h \
    salesordermanager.h \
    salesordermodel.h \
    salesorderproxymodel.h \
    settingsdialog.h

FORMS += \
    aboutdialog.ui \
    calculatordialog.ui \
    dashboard.ui \
    datechooserdialog.ui \
    logindialog.ui \
    mainwindow.ui \
    productcategoryeditor.ui \
    productcategorymanager.ui \
    producteditor.ui \
    productmanager.ui \
    purchaseordereditor.ui \
    purchaseorderitemeditor.ui \
    purchaseordermanager.ui \
    reportdatedialog.ui \
    salesordereditor.ui \
    salesorderitemeditor.ui \
    salesordermanager.ui \
    settingsdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
DESTDIR = $$PWD/../../deploy
TARGET = simple-pos

RESOURCES += \
    app.qrc

DISTFILES += \
    app.ico \
    app.rc \
    resources/reports/kwitansi.html \
    resources/reports/lap-penjualan-bulanan-per-tanggal.html \
    resources/reports/lap-penjualan-harian-per-produk.html \
    resources/reports/lap-rincian-penjualan-harian.html
