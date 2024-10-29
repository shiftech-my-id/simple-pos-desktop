QT += core gui widgets sql printsupport
CONFIG += c++17 fontAwesomeFree
include(../libs/QtAwesome/QtAwesome.pri)
RC_FILE = app.rc
SOURCES += \
    aboutdialog.cpp \
    application.cpp \
    calculatordialog.cpp \
    changepassworddialog.cpp \
    common.cpp \
    dashboard.cpp \
    datechooserdialog.cpp \
    db.cpp \
    libs/currencyedit.cpp \
    libs/numberedit.cpp \
    libs/tabbar.cpp \
    libs/tableview.cpp \
    libs/tabwidget.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    productcategoryeditor.cpp \
    productcategorymanager.cpp \
    productcategorymodel.cpp \
    productcategoryproxymodel.cpp \
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
    settingsdialog.cpp \
    suppliereditor.cpp \
    suppliermanager.cpp \
    usereditor.cpp \
    usermanager.cpp \
    usermodel.cpp \
    userproxymodel.cpp \
    suppliermodel.cpp \
    supplierproxymodel.cpp

HEADERS += \
    aboutdialog.h \
    application.h \
    calculatordialog.h \
    changepassworddialog.h \
    common.h \
    dashboard.h \
    datechooserdialog.h \
    db.h \
    global.h \
    libs/currencyedit.h \
    libs/numberedit.h \
    libs/tabbar.h \
    libs/tableview.h \
    libs/tabwidget.h \
    logindialog.h \
    mainwindow.h \
    productcategoryeditor.h \
    productcategorymanager.h \
    productcategorymodel.h \
    productcategoryproxymodel.h \
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
    settingsdialog.h \
    suppliereditor.h \
    suppliermanager.h \
    usereditor.h \
    usermanager.h \
    usermodel.h \
    userproxymodel.h \
    suppliermodel.h \
    supplierproxymodel.h

FORMS += \
    aboutdialog.ui \
    calculatordialog.ui \
    changepassworddialog.ui \
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
    settingsdialog.ui \
    suppliereditor.ui \
    suppliermanager.ui \
    usereditor.ui \
    usermanager.ui

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
