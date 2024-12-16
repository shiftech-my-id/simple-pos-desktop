QT += widgets sql printsupport network
CONFIG += c++17 fontAwesomeFree
LIBS += -lole32 -loleaut32
RC_FILE = app.rc
SOURCES += \
    db/migrationmanager.cpp \
    db/usertable.cpp \
    entity/abstractentity.cpp \
    entity/address.cpp \
    entity/customer.cpp \
    entity/party.cpp \
    entity/product.cpp \
    entity/productcategory.cpp \
    entity/stockupdate.cpp \
    entity/stockupdatedetail.cpp \
    entity/supplier.cpp \
    entity/user.cpp \
    db/db.cpp \
    db/partytable.cpp \
    db/productcategorytable.cpp \
    db/producttable.cpp \
    db/settingtable.cpp \
    db/table.cpp \
    gui/customermodel.cpp \
    gui/customerproxymodel.cpp \
    gui/productcategorymodel.cpp \
    gui/productcategoryproxymodel.cpp \
    gui/productmodel.cpp \
    gui/productproxymodel.cpp \
    gui/purchaseordereditormodel.cpp \
    gui/purchaseorderproxymodel.cpp \
    gui/suppliermodel.cpp \
    gui/supplierproxymodel.cpp \
    gui/usermodel.cpp \
    gui/userproxymodel.cpp \
    gui/purchaseordermodel.cpp \
    gui/salesordermodel.cpp \
    gui/salesorderproxymodel.cpp \
    common.cpp \
    main.cpp \
    widgets/calculatordialog.cpp \
    widgets/changepassworddialog.cpp \
    widgets/customer/customereditor.cpp \
    widgets/customer/customermanager.cpp \
    widgets/dashboard/dashboard.cpp \
    widgets/datechooserdialog.cpp \
    widgets/logindialog.cpp \
    widgets/mainwindow.cpp \
    widgets/application.cpp \
    widgets/aboutdialog.cpp \
    widgets/product/productcategoryeditor.cpp \
    widgets/product/productcategorymanager.cpp \
    widgets/product/producteditor.cpp \
    widgets/product/productmanager.cpp \
    widgets/purchasing/purchaseordereditor.cpp \
    widgets/purchasing/purchaseorderitemeditor.cpp \
    widgets/purchasing/purchaseordermanager.cpp \
    widgets/reportdatedialog.cpp \
    widgets/sales/salesordereditor.cpp \
    widgets/sales/salesordereditormodel.cpp \
    widgets/sales/salesorderitemeditor.cpp \
    widgets/sales/salesordermanager.cpp \
    widgets/settings/activationdialog.cpp \
    widgets/settings/companysettingswidget.cpp \
    widgets/settings/printersettingswidget.cpp \
    widgets/settings/settingsdialog.cpp \
    widgets/supplier/suppliereditor.cpp \
    widgets/supplier/suppliermanager.cpp \
    widgets/user/usereditor.cpp \
    widgets/user/usermanager.cpp

HEADERS += \
    db/migrationmanager.h \
    db/usertable.h \
    entity/abstractentity.h \
    entity/address.h \
    entity/customer.h \
    entity/party.h \
    entity/product.h \
    entity/productcategory.h \
    entity/stockupdate.h \
    entity/stockupdatedetail.h \
    entity/supplier.h \
    entity/user.h \
    db/db.h \
    db/partytable.h \
    db/productcategorytable.h \
    db/producttable.h \
    db/settingtable.h \
    db/table.h \
    gui/customermodel.h \
    gui/customerproxymodel.h \
    gui/productcategorymodel.h \
    gui/productcategoryproxymodel.h \
    gui/productmodel.h \
    gui/productproxymodel.h \
    gui/purchaseordereditormodel.h \
    gui/purchaseorderproxymodel.h \
    gui/suppliermodel.h \
    gui/supplierproxymodel.h \
    gui/usermodel.h \
    gui/userproxymodel.h \
    gui/purchaseordermodel.h \
    gui/salesordermodel.h \
    gui/salesorderproxymodel.h \
    widgets/aboutdialog.h \
    widgets/application.h \
    common.h \
    widgets/calculatordialog.h \
    widgets/changepassworddialog.h \
    widgets/customer/customereditor.h \
    widgets/customer/customermanager.h \
    widgets/dashboard/dashboard.h \
    widgets/datechooserdialog.h \
    widgets/logindialog.h \
    widgets/mainwindow.h \
    widgets/product/productcategoryeditor.h \
    widgets/product/productcategorymanager.h \
    widgets/product/producteditor.h \
    widgets/product/productmanager.h \
    widgets/purchasing/purchaseordereditor.h \
    widgets/purchasing/purchaseorderitemeditor.h \
    widgets/purchasing/purchaseordermanager.h \
    widgets/reportdatedialog.h \
    widgets/sales/salesordereditor.h \
    widgets/sales/salesordereditormodel.h \
    widgets/sales/salesorderitemeditor.h \
    widgets/sales/salesordermanager.h \
    widgets/settings/activationdialog.h \
    widgets/settings/companysettingswidget.h \
    widgets/settings/printersettingswidget.h \
    widgets/settings/settingsdialog.h \
    widgets/supplier/suppliereditor.h \
    widgets/supplier/suppliermanager.h \
    widgets/user/usereditor.h \
    widgets/user/usermanager.h

FORMS += \
    widgets/aboutdialog.ui \
    widgets/calculatordialog.ui \
    widgets/changepassworddialog.ui \
    widgets/customer/customereditor.ui \
    widgets/customer/customermanager.ui \
    widgets/dashboard/dashboard.ui \
    widgets/datechooserdialog.ui \
    widgets/logindialog.ui \
    widgets/mainwindow.ui \
    widgets/product/productcategoryeditor.ui \
    widgets/product/productcategorymanager.ui \
    widgets/product/producteditor.ui \
    widgets/product/productmanager.ui \
    widgets/purchasing/purchaseordereditor.ui \
    widgets/purchasing/purchaseorderitemeditor.ui \
    widgets/purchasing/purchaseordermanager.ui \
    widgets/reportdatedialog.ui \
    widgets/sales/salesordereditor.ui \
    widgets/sales/salesorderitemeditor.ui \
    widgets/sales/salesordermanager.ui \
    widgets/settings/activationdialog.ui \
    widgets/settings/companysettingswidget.ui \
    widgets/settings/printersettingswidget.ui \
    widgets/settings/settingsdialog.ui \
    widgets/supplier/suppliereditor.ui \
    widgets/supplier/suppliermanager.ui \
    widgets/user/usereditor.ui \
    widgets/user/usermanager.ui


include(../../libs/QtAwesome/QtAwesome.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

TARGET = simple-pos

RESOURCES += app.qrc

DISTFILES += \
    CODE_REVIEW.txt \
    app.ico \
    app.rc \
    resources/migrations/001_init.sql \
    resources/migrations/002_something.sql \
    resources/reports/kwitansi.html \
    resources/reports/lap-penjualan-bulanan-per-tanggal.html \
    resources/reports/lap-penjualan-harian-per-produk.html \
    resources/reports/lap-rincian-penjualan-harian.html

DESTDIR = ../../../deploy

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
