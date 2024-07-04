#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "productmanager.h"
#include "salesordermanager.h"
#include "purchaseordermanager.h"
#include "productcategorymanager.h"
#include "settingsdialog.h"
#include "reportdatedialog.h"
#include "datechooserdialog.h"
#include "dashboard.h"
#include "db.h"

#include <QPrinter>
#include <QTabWidget>
#include <QLabel>
#include <QToolBar>
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QSettings>
#include <QFile>
#include <QTextFrame>
#include <QTextFrameFormat>
#include <QTextCursor>
#include <QSqlRecord>
#include <QSqlError>
#include <QTextDocument>
#include <QTabBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tabWidget(new QTabWidget(this)),
    dashboard(new Dashboard(this)),
    productManager(nullptr),
    productCategoryManager(nullptr),
    salesOrderManager(nullptr),
    purchaseOrderManager(nullptr)
{
    ui->setupUi(this);

    tabWidget->setTabsClosable(true);
    tabWidget->addTab(dashboard, "Dasbor");
    setCentralWidget(tabWidget);
    QTabBar * tabBar = tabWidget->tabBar();
    tabBar->tabButton(0, QTabBar::RightSide)->deleteLater();
    tabBar->setTabButton(0, QTabBar::RightSide, nullptr);

    QLabel *label = new QLabel(this);
    label->setText("  © Shift Komputer 2022");

    QLabel *label2 = new QLabel(this);
    label2->setText(QString("%1 v%2 berbasis Qt %3").arg(APP_DISPLAY_NAME, APP_VERSION_STR, QT_VERSION_STR));

    ui->statusbar->addWidget(label, 0);
    ui->statusbar->addWidget(new QLabel(this), 1);
    ui->statusbar->addWidget(label2, 0);

    connect(tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::closeTab);
    connect(ui->productAction, &QAction::triggered, this, &MainWindow::showProductManager);
    connect(ui->productCategoryAction, &QAction::triggered, this, &MainWindow::showProductCategoryManager);
    connect(ui->salesAction, &QAction::triggered, this, &MainWindow::showSalesOrderManager);
    connect(ui->purchasingAction, &QAction::triggered, this, &MainWindow::showPurchaseOrderManager);
    connect(ui->settingsAction, &QAction::triggered, this, &MainWindow::showSettings);
    connect(ui->reportSalesDetailDailyAction, SIGNAL(triggered()), SLOT(printReportSalesDetailDaily()));
    connect(ui->reportSalesDetailAction, SIGNAL(triggered()), SLOT(printReportSalesDetail()));
    connect(ui->reportMonthlySalesAction, SIGNAL(triggered()), SLOT(printReportMonthlySales()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showProductCategoryManager()
{
    if (productCategoryManager == nullptr) {
        productCategoryManager = new ProductCategoryManager(this);
        tabWidget->addTab(productCategoryManager, "Kategori Produk");
    }
    tabWidget->setCurrentWidget(productCategoryManager);
}

void MainWindow::showProductManager()
{
    if (productManager == nullptr) {
        productManager = new ProductManager(this);
        tabWidget->addTab(productManager, "Produk");
    }
    tabWidget->setCurrentWidget(productManager);
}

void MainWindow::showSalesOrderManager()
{
    if (salesOrderManager == nullptr) {
        salesOrderManager = new SalesOrderManager(this);
        tabWidget->addTab(salesOrderManager, "Penjualan");
    }
    tabWidget->setCurrentWidget(salesOrderManager);
}

void MainWindow::showPurchaseOrderManager()
{
    if (purchaseOrderManager == nullptr) {
        purchaseOrderManager = new PurchaseOrderManager(this);
        tabWidget->addTab(purchaseOrderManager, "Pembelian");
    }
    tabWidget->setCurrentWidget(purchaseOrderManager);
}

void MainWindow::showSettings()
{
    SettingsDialog dialog;
    dialog.setWindowTitle("Pengaturan");
    dialog.exec();
}

void MainWindow::closeTab(int index)
{
    QWidget* w = tabWidget->widget(index);
    if (w == dashboard) {
        return;
    }

    if (!w->close()) {
        return;
    }

    tabWidget->removeTab(index);
    w->deleteLater();

    if (w == productManager) {
        productManager = nullptr;
    }
    else if (w == productCategoryManager) {
        productCategoryManager = nullptr;
    }
    else if (w == purchaseOrderManager) {
        purchaseOrderManager = nullptr;
    }
    else if (w == salesOrderManager) {
        salesOrderManager = nullptr;
    }
}

void MainWindow::printReportSalesDetailDaily(QPrinter *printer)
{
    if (printer == nullptr) {
        DateChooserDialog dateChooserDialog(this);
        dateChooserDialog.setWindowTitle("Laporan Rincian Penjualan Harian");
        if (!dateChooserDialog.exec()) {
            return;
        }

        setProperty("tmp_date", dateChooserDialog.selectedDate());

        QPrinter printer(QPrinter::ScreenResolution);
        printer.setColorMode(QPrinter::GrayScale);
#if QT_VERSION >= 0x060000
        printer.setPageSize(QPageSize::A4);
#else
        printer.setPageSize(QPagedPaintDevice::A4);
#endif
        printer.setPageOrientation(QPageLayout::Portrait);
        printer.setFullPage(false);
        printer.setPageMargins(QMarginsF(10, 10, 10, 10), QPageLayout::Millimeter);

        QPrintPreviewDialog dialog(&printer, this, Qt::WindowCloseButtonHint | Qt::WindowMinMaxButtonsHint);
        connect(&dialog, SIGNAL(paintRequested(QPrinter*)), SLOT(printReportSalesDetailDaily(QPrinter*)));
        dialog.setWindowTitle("Cetak Laporan");
        dialog.setWindowState(Qt::WindowMaximized);

        for (QAction *action: dialog.findChildren<QAction*>()) {
            if (action->text() == "Page setup")
                action->setEnabled(false);
            if (action->text() == "Landscape")
                action->setEnabled(false);
            if (action->text() == "Portrait")
                action->setEnabled(false);
        }
        QToolBar *toolBar = dialog.findChild<QToolBar*>();
        toolBar->setMovable(false);
        toolBar->setIconSize(QSize(16, 16));

        dialog.exec();
        return;
    }

    const QDate date = property("tmp_date").toDate();

    QSettings settings("settings.ini", QSettings::IniFormat);

    QFile file("lap-rincian-penjualan-harian.html");
    file.open(QFile::ReadOnly | QFile::Text);

    QString html = QString::fromUtf8(file.readAll());
    settings.beginGroup("CompanyInfo");
    html.replace("{STORE_NAME}", settings.value("store_name").toString());
    html.replace("{STORE_ADDRESS}", settings.value("store_address").toString());
    html.replace("{DATE}", locale().toString(date, "d MMMM yyyy"));
    settings.endGroup();

    QSqlDatabase db = db::database();
    QSqlQuery q(db);

    q.prepare("select d.* from stock_update_details d"
              " inner join stock_updates o on o.id = d.update_id"
              " where date(o.datetime)=:date"
              " order by update_id asc, id asc");
    q.bindValue(":date", date);
    q.exec();
    QMultiMap<int, StockUpdateDetail> itemsBySalesId;
    while (q.next()) {
        StockUpdateDetail d;
        d.id = q.value("id").toInt();
        d.productName = q.value("product_name").toString();
        d.productUom = q.value("product_uom").toString();
        d.quantity = q.value("quantity").toInt();
        d.price = q.value("price").toDouble();
        d.subtotalPrice = q.value("subtotal_price").toDouble();
        itemsBySalesId.insert(q.value("update_id").toInt(), d);
    }

    q.prepare("select * from stock_updates where date(datetime)=:date");
    q.bindValue(":date", date);
    q.exec();

    QString items;
    int num = 0;
    double total = 0.;
    double totalDiscount = 0.;
    double totalProfit = 0.;
    while (q.next()) {
        total += q.value("total").toDouble();
        totalDiscount += q.value("discount").toDouble();
        totalProfit += q.value("total_profit").toDouble();
        num++;
        items.append(QString(
            "<tr>"
                "<td align=\"center\">%1</td>"
                "<td align=\"left\">%2 %3 | %4 - %5</td>"
                "<td align=\"right\">%6</td>"
                "<td align=\"right\">%7</td>"
                "<td align=\"right\">%8</td>"
            "</tr>")
        .arg(
            locale().toString(num),
            StockUpdate::salesOrderCode(q.value("id").toInt()),
            locale().toString(q.value("datetime").toDateTime(), "HH:mm:ss"),
            q.value("party_name").toString(),
            q.value("party_address").toString(),
            locale().toString(q.value("total").toDouble(), 'f', 0),
            locale().toString(q.value("discount").toDouble(), 'f', 0),
            locale().toString(q.value("total_profit").toDouble(), 'f', 0)
        ));

        int num2 = 0;
        for (const StockUpdateDetail& d: itemsBySalesId.values(q.value("id").toInt())) {
            num2++;
            items.append(QString(
                "<tr>"
                    "<td></td>"
                    "<td colspan=4>%1 - %2 %3 %4 @ %5 = %6</td>"
                "</tr>"
            ).arg(
                QString::number(num2),
                QString::number(d.quantity),
                d.productUom,
                d.productName,
                locale().toString(d.price, 'f', 0),
                locale().toString(d.subtotalPrice, 'f', 0)
            ));
        }
        items.append("<tr><td colspan=5></td></tr>");
    }
    if (items.isEmpty()) {
        items.append("<tr><td colspan=5 align=center><i>Tidak ada data untuk ditampilkan</i></td></tr>");
    }
    html.replace("{ITEMS}", items);
    html.replace("{TOTAL_SALES}", locale().toString(total, 'f', 0));
    html.replace("{TOTAL_DISCOUNT}", locale().toString(totalDiscount, 'f', 0));
    html.replace("{TOTAL_PROFIT}", locale().toString(totalProfit, 'f', 0));

    QTextDocument doc;
    QFont defaultFont = doc.defaultFont();
    defaultFont.setFamily("Arial Narrow");
    defaultFont.setPointSize(9);
    defaultFont.setStretch(101);
    doc.setDefaultFont(defaultFont);

    QTextFrameFormat fmt = doc.rootFrame()->frameFormat();
    fmt.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
    fmt.setMargin(0);
    fmt.setPadding(0);
    doc.rootFrame()->setFrameFormat(fmt);

    QRectF pageRect(printer->pageRect(QPrinter::DevicePixel));
    QRectF body = QRectF(0, 0, pageRect.width(), pageRect.height());
    doc.setPageSize(body.size());

    QTextCursor cursor(&doc);
    cursor.insertHtml(html);

    printer->setDocName("Laporan Penjualan Harian" + locale().toString(date));
    doc.print(printer);
}

void MainWindow::printReportSalesDetail(QPrinter *printer)
{
    if (printer == nullptr) {
        DateChooserDialog dateChooserDialog(this);
        dateChooserDialog.setWindowTitle("Laporan Rekap Penjualan Harian");
        if (!dateChooserDialog.exec()) {
            return;
        }

        setProperty("tmp_date", dateChooserDialog.selectedDate());

        QPrinter printer(QPrinter::ScreenResolution);
        printer.setColorMode(QPrinter::GrayScale);
#if QT_VERSION >= 0x060000
        printer.setPageSize(QPageSize::A4);
#else
        printer.setPageSize(QPagedPaintDevice::A4);
#endif
        printer.setPageOrientation(QPageLayout::Portrait);
        printer.setFullPage(false);
        printer.setPageMargins(QMarginsF(10, 10, 10, 10), QPageLayout::Millimeter);

        QPrintPreviewDialog dialog(&printer, this, Qt::WindowCloseButtonHint | Qt::WindowMinMaxButtonsHint);
        connect(&dialog, SIGNAL(paintRequested(QPrinter*)), SLOT(printReportSalesDetail(QPrinter*)));
        dialog.setWindowTitle("Cetak Laporan");
        dialog.setWindowState(Qt::WindowMaximized);

        for (QAction *action: dialog.findChildren<QAction*>()) {
            if (action->text() == "Page setup")
                action->setEnabled(false);
            if (action->text() == "Landscape")
                action->setEnabled(false);
            if (action->text() == "Portrait")
                action->setEnabled(false);
        }
        QToolBar *toolBar = dialog.findChild<QToolBar*>();
        toolBar->setMovable(false);
        toolBar->setIconSize(QSize(16, 16));

        dialog.exec();
        return;
    }

    const QDate date = property("tmp_date").toDate();

    QSettings settings("settings.ini", QSettings::IniFormat);

    QFile file("lap-penjualan-harian-per-produk.html");
    file.open(QFile::ReadOnly | QFile::Text);

    QString html = QString::fromUtf8(file.readAll());
    settings.beginGroup("CompanyInfo");
    html.replace("{STORE_NAME}", settings.value("store_name").toString());
    html.replace("{STORE_ADDRESS}", settings.value("store_address").toString());
    html.replace("{DATE}", locale().toString(date, "d MMMM yyyy"));
    settings.endGroup();

    QSqlDatabase db = db::database();
    QSqlQuery q(db);
    q.prepare("select d.*"
              " from stock_update_details d"
              " inner join stock_updates o on o.id = d.update_id"
              " where date(o.datetime)=:date");
    q.bindValue(":date", date);
    q.exec();

    QMap<QString, QVariantMap> itemByIds;
    while (q.next()) {
        QString productName = q.value("product_name").toString();
        QVariantMap data;
        if (!itemByIds.contains(productName)) {
            data["name"] = productName;
            data["uom"] = q.value("product_uom").toString();
            data["quantity"] = 0;
            data["cost"] = 0.;
            data["price"] = 0.;
        }
        else {
            data = itemByIds.value(productName);
        }
        data["quantity"] = data["quantity"].toInt() + q.value("quantity").toInt();
        data["cost"] = data["cost"].toDouble() + q.value("subtotal_cost").toDouble();
        data["price"] = data["price"].toDouble() + q.value("subtotal_price").toDouble();
        itemByIds[productName] = data;
    }

    QString items;
    int num = 0;
    int totalQty = 0;
    double totalCost = 0.;
    double totalPrice = 0.;
    double totalProfit = 0.;
    for (const QString& productName : itemByIds.keys()) {
        QVariantMap data = itemByIds.value(productName);
        int quantity = data.value("quantity").toInt();
        double cost = data.value("cost").toDouble();
        double price = data.value("price").toDouble();
        double profit = price - cost;
        totalQty += quantity;
        totalCost += cost;
        totalPrice += price;
        totalProfit += profit;
        num++;
        items.append(QString(
            "<tr>"
                "<td align=center>%1</td>"
                "<td align=left>%2</td>"
                "<td align=center>%3 %4</td>"
                "<td align=right>%5</td>"
                "<td align=right>%6</td>"
                "<td align=right>%7</td>"
            "</tr>")
        .arg(
            locale().toString(num),
            productName,
            locale().toString(quantity),
            data.value("uom").toString(),
            locale().toString(cost, 'f', 0),
            locale().toString(price, 'f', 0),
            locale().toString(profit, 'f', 0)
        ));
    }
    if (items.isEmpty()) {
        items.append("<tr><td colspan=6 align=center><i>Tidak ada data untuk ditampilkan</i></td></tr>");
    }
    html.replace("{ITEMS}", items);
    html.replace("{TOTAL_QUANTITY}", locale().toString(totalQty));
    html.replace("{TOTAL_COST}", locale().toString(totalCost, 'f', 0));
    html.replace("{TOTAL_PRICE}", locale().toString(totalPrice, 'f', 0));
    html.replace("{TOTAL_PROFIT}", locale().toString(totalProfit, 'f', 0));

    QTextDocument doc;
    QFont defaultFont = doc.defaultFont();
    defaultFont.setFamily("Arial Narrow");
    defaultFont.setPointSize(9);
    defaultFont.setStretch(101);
    doc.setDefaultFont(defaultFont);

    QTextFrameFormat fmt = doc.rootFrame()->frameFormat();
    fmt.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
    fmt.setMargin(0);
    fmt.setPadding(0);
    doc.rootFrame()->setFrameFormat(fmt);

    QRectF pageRect(printer->pageRect(QPrinter::DevicePixel));
    QRectF body = QRectF(0, 0, pageRect.width(), pageRect.height());
    doc.setPageSize(body.size());

    QTextCursor cursor(&doc);
    cursor.insertHtml(html);

    printer->setDocName("Laporan Penjualan Harian" + locale().toString(date));
    doc.print(printer);
}

void MainWindow::printReportMonthlySales(QPrinter *printer)
{
    if (printer == nullptr) {
        ReportDateDialog dateDialog(this);
        dateDialog.setWindowTitle("Laporan Rekap Penjualan Bulanan");
        if (!dateDialog.exec()) {
            return;
        }

        int year = dateDialog.year();
        int month = dateDialog.month();
        QDate date(year, month, 1);
        setProperty("tmp_date", date);

        QPrinter printer(QPrinter::ScreenResolution);
        printer.setColorMode(QPrinter::GrayScale);
#if QT_VERSION >= 0x060000
        printer.setPageSize(QPageSize::A4);
#else
        printer.setPageSize(QPagedPaintDevice::A4);
#endif
        printer.setPageOrientation(QPageLayout::Portrait);
        printer.setFullPage(false);
        printer.setPageMargins(QMarginsF(10, 10, 10, 10), QPageLayout::Millimeter);

        QPrintPreviewDialog dialog(&printer, this, Qt::WindowCloseButtonHint | Qt::WindowMinMaxButtonsHint);
        connect(&dialog, SIGNAL(paintRequested(QPrinter*)), SLOT(printReportMonthlySales(QPrinter*)));
        dialog.setWindowTitle("Cetak Laporan");
        dialog.setWindowState(Qt::WindowMaximized);

        for (QAction *action: dialog.findChildren<QAction*>()) {
            if (action->text() == "Page setup")
                action->setEnabled(false);
            if (action->text() == "Landscape")
                action->setEnabled(false);
            if (action->text() == "Portrait")
                action->setEnabled(false);
        }
        QToolBar *toolBar = dialog.findChild<QToolBar*>();
        toolBar->setMovable(false);
        toolBar->setIconSize(QSize(16, 16));

        dialog.exec();
        return;
    }

    const QDate date = property("tmp_date").toDate();

    QSettings settings("settings.ini", QSettings::IniFormat);

    QFile file("lap-penjualan-bulanan-per-tanggal.html");
    file.open(QFile::ReadOnly | QFile::Text);

    QString html = QString::fromUtf8(file.readAll());
    settings.beginGroup("CompanyInfo");
    html.replace("{STORE_NAME}", settings.value("store_name").toString());
    html.replace("{STORE_ADDRESS}", settings.value("store_address").toString());
    html.replace("{DATE}", locale().toString(date, "MMMM yyyy"));
    settings.endGroup();

    QSqlDatabase db = db::database();
    QSqlQuery q(db);
    q.prepare("select *"
              " from stock_updates o"
              " where date(o.datetime) between :startDate and :endDate");
    q.bindValue(":startDate", date);
    q.bindValue(":endDate", QDate(date.year(), date.month(), date.daysInMonth()));
    q.exec();

    QMap<int, QVariantMap> itemByDates;
    while (q.next()) {
        int day = q.value("datetime").toDateTime().date().day();
        QVariantMap data;
        if (!itemByDates.contains(day)) {
            data["cost"]     = 0.;
            data["price"]    = 0.;
            data["discount"] = 0.;
            data["profit"]   = 0.;
        }
        else {
            data = itemByDates.value(day);
        }
        data["cost"]     = data["cost"].toDouble()     + q.value("total_cost").toDouble();
        data["price"]    = data["price"].toDouble()    + q.value("total").toDouble();
        data["discount"] = data["discount"].toDouble() + q.value("discount").toDouble();
        data["profit"]   = data["profit"].toDouble()   + q.value("total_profit").toDouble();
        itemByDates[day] = data;
    }

    QString items;
    double totalCost = 0.;
    double totalPrice = 0.;
    double totalDiscount = 0.;
    double totalProfit = 0.;
    for (const int day : itemByDates.keys()) {
        QVariantMap data = itemByDates.value(day);
        double cost = data.value("cost").toDouble();
        double price = data.value("price").toDouble();
        double discount = data.value("discount").toDouble();
        double profit = data.value("profit").toDouble();
        totalCost += cost;
        totalPrice += price;
        totalDiscount += discount;
        totalProfit += profit;
        items.append(QString(
            "<tr>"
                "<td align=center>%1</td>"
                "<td align=right>%2</td>"
                "<td align=right>%3</td>"
                "<td align=right>%4</td>"
                "<td align=right>%5</td>"
            "</tr>")
        .arg(
            locale().toString(day),
            locale().toString(cost, 'f', 0),
            locale().toString(price, 'f', 0),
            locale().toString(discount, 'f', 0),
            locale().toString(profit, 'f', 0)
        ));
    }

    if (itemByDates.isEmpty()) {
        items.append("<tr><td colspan=5 align=center><i>Tidak ada data untuk ditampilkan</i></td></tr>");
    }

    html.replace("{ITEMS}", items);
    html.replace("{TOTAL_COST}", locale().toString(totalCost, 'f', 0));
    html.replace("{TOTAL_PRICE}", locale().toString(totalPrice, 'f', 0));
    html.replace("{TOTAL_DISCOUNT}", locale().toString(totalDiscount, 'f', 0));
    html.replace("{TOTAL_PROFIT}", locale().toString(totalProfit, 'f', 0));


    QTextDocument doc;
    QFont defaultFont = doc.defaultFont();
    defaultFont.setFamily("Arial Narrow");
    defaultFont.setPointSize(9);
    defaultFont.setStretch(101);
    doc.setDefaultFont(defaultFont);

    QTextFrameFormat fmt = doc.rootFrame()->frameFormat();
    fmt.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
    fmt.setMargin(0);
    fmt.setPadding(0);
    doc.rootFrame()->setFrameFormat(fmt);

    QRectF pageRect(printer->pageRect(QPrinter::DevicePixel));
    QRectF body = QRectF(0, 0, pageRect.width(), pageRect.height());
    doc.setPageSize(body.size());

    QTextCursor cursor(&doc);
    cursor.insertHtml(html);

    printer->setDocName("Laporan Penjualan Harian" + locale().toString(date));
    doc.print(printer);
}
