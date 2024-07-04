#include "salesordereditor.h"
#include "ui_salesordereditor.h"
#include "salesordereditormodel.h"
#include "salesorderitemeditor.h"

#include "db.h"
#include <QSettings>
#include <QMessageBox>
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QToolBar>
#include <QPainter>
#include <QAbstractTextDocumentLayout>
#include <QTextFrameFormat>
#include <QTextFrame>
#include <QFile>
#include <QComboBox>

SalesOrderEditor::SalesOrderEditor(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint | Qt::WindowMinMaxButtonsHint),
    ui(new Ui::SalesOrderEditor),
    model(new SalesOrderEditorModel(this))
{
    ui->setupUi(this);

    editor = new SalesOrderItemEditor;
    connect(editor, &SalesOrderItemEditor::itemAdded, model, &SalesOrderEditorModel::addItem);

    editor->adjustSize();
    ui->itemsLayout->insertWidget(0, editor);

    ui->tableView->setModel(model);
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());

    QRegularExpressionValidator *numberValidator = new QRegularExpressionValidator(
        QRegularExpression(QString("^\\d*|[1-9]\\d{0,2}(\\%1\\d{3})*(%2\\d+)?$")
                           .arg(locale().groupSeparator(), locale().decimalPoint())), this);
    ui->totalPaidEdit->setValidator(numberValidator);
    ui->discountEdit->setValidator(numberValidator);

    ui->totalPaidEdit->installEventFilter(this);
    ui->discountEdit->installEventFilter(this);

    connect(ui->saveButton, SIGNAL(clicked()), SLOT(accept()));
    connect(ui->printButton, SIGNAL(clicked()), SLOT(saveAndPrint()));
    connect(ui->cancelButton, SIGNAL(clicked()), SLOT(reject()));
    connect(model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), SLOT(onModelChanged()));
    connect(model, SIGNAL(rowsRemoved(QModelIndex,int,int)), SLOT(onModelChanged()));
    connect(model, SIGNAL(rowsInserted(QModelIndex,int,int)), SLOT(onModelChanged()));
    connect(model, SIGNAL(rowsRemoved(QModelIndex,int,int)), SLOT(updateInfoLabel()));
    connect(model, SIGNAL(rowsInserted(QModelIndex,int,int)), SLOT(updateInfoLabel()));
    connect(ui->discountEdit, SIGNAL(editingFinished()), SLOT(updateTotal()));
    connect(ui->totalPaidEdit, SIGNAL(editingFinished()), SLOT(updateExchange()));
    connect(ui->discountEdit, SIGNAL(returnPressed()), SLOT(focusToPayment()));
    connect(ui->totalPaymentEdit, SIGNAL(returnPressed()), ui->saveButton, SLOT(setFocus()));

    removeAction = new QAction(this);
    addAction(removeAction);
    removeAction->setShortcut(Qt::Key_Delete);
    connect(removeAction, &QAction::triggered, this, &SalesOrderEditor::remove);

    QAction* discountAction = new QAction(this);
    addAction(discountAction);
    discountAction->setShortcut(Qt::Key_F9);
    connect(discountAction, &QAction::triggered, this, &SalesOrderEditor::focusToDiscount);

    QAction* paymentAction = new QAction(this);
    addAction(paymentAction);
    paymentAction->setShortcut(Qt::Key_F10);
    connect(paymentAction, &QAction::triggered, this, &SalesOrderEditor::focusToPayment);

    saveAction = new QAction(this);
    addAction(saveAction);
    saveAction->setShortcut(Qt::Key_F11);
    connect(saveAction, &QAction::triggered, this, &SalesOrderEditor::save);

    QAction* printAction = new QAction(this);
    addAction(printAction);
    printAction->setShortcut(Qt::Key_F12);
    connect(printAction, &QAction::triggered, this, &SalesOrderEditor::saveAndPrint);

    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    editor->findChild<QComboBox*>("productComboBox")->lineEdit()->setFocus();
}

SalesOrderEditor::~SalesOrderEditor()
{
    delete ui;
}

void SalesOrderEditor::reject()
{
    if (salesOrder.id == 0) {
        if (msgBoxQuestion(this, "Konfimasi", QString("Batalkan transaksi penjualan?")) != QMessageBox::Yes) {
            return;
        }
    }

    return QDialog::reject();
}

bool SalesOrderEditor::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::FocusOut) {
        if (watched == ui->totalPaidEdit || watched == ui->discountEdit) {
            fixNumber(watched);
        }
    }
    if (event->type() == QEvent::KeyPress && static_cast<QKeyEvent*>(event)->key() == Qt::Key_Return) {
        if (watched == ui->discountEdit) {
            focusToPayment();
            return true;
        }
        else if (watched == ui->totalPaidEdit) {
            ui->saveButton->setFocus();
            return true;
        }
    }
    return QDialog::eventFilter(watched, event);
}

void SalesOrderEditor::fixNumber(QObject *sender)
{
    QLineEdit *lineEdit = static_cast<QLineEdit*>(sender);
    QString text = lineEdit->text().trimmed();

    if (text.isEmpty()) {
        text = "0";
    }
    else if (text.endsWith(".") || text.endsWith(",")) {
        text = text.left(text.size() - 1);
    }
    else {
        text = locale().toString(locale().toDouble(text), 'f', 0);
    }

    lineEdit->setText(text);
}

void SalesOrderEditor::add()
{
    SalesOrderItemEditor editor(this);
    connect(&editor, SIGNAL(itemAdded(StockUpdateDetail)), model, SLOT(addItem(StockUpdateDetail)));
    editor.exec();
}

void SalesOrderEditor::remove()
{
    if (salesOrder.id) {
        return;
    }

    QModelIndex index = ui->tableView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Peringatan", "Anda belum memilih item yang akan dihapus.");
        return;
    }

    if (msgBoxQuestion(this, "Konfirmasi", "Hapus item yang sedang dipilih?") != QMessageBox::Yes) {
        return;
    }

    model->removeRow(index.row());
    ui->tableView->clearSelection();
}

bool SalesOrderEditor::save()
{
    if (msgBoxQuestion(this, "Konfirmasi", "Simpan transaksi penjualan?") != QMessageBox::Yes) {
        return false;
    }

    if (salesOrder.id) {
        // PENGEDITAN TIDAK DIPERBOLEHKAN
        return true;
    }

    if (model->rowCount() == 0) {
        QMessageBox::information(this, "Informasi", "Belum ada item yang ditambahkan.");
        return false;
    }

    salesOrder.partyName = ui->customerNameEdit->text().trimmed();
    salesOrder.partyAddress = ui->customerAddressEdit->text().trimmed();
    salesOrder.dateTime = ui->dateTimeEdit->dateTime();
    salesOrder.total = locale().toDouble(ui->totalEdit->text());
    salesOrder.discount = locale().toDouble(ui->discountEdit->text());
    salesOrder.totalPayment = locale().toDouble(ui->totalPaymentEdit->text());
    salesOrder.totalPaid = locale().toDouble(ui->totalPaidEdit->text());
    salesOrder.exchange = locale().toDouble(ui->exchangeEdit->text());

    if (salesOrder.totalPaid < salesOrder.totalPayment) {
        QMessageBox::information(this, "Informasi", "Jumlah bayar tidak boleh kurang dari tagihan.");
        ui->totalPaidEdit->setFocus();
        ui->totalPaidEdit->selectAll();
        return false;
    }

    salesOrder.totalCost = 0.;
    salesOrder.totalProfit = 0.;
    for (StockUpdateDetail &item: model->items) {
        salesOrder.totalCost += item.subtotalCost;
        salesOrder.totalProfit += (item.subtotalPrice - item.subtotalCost);
    }
    salesOrder.totalProfit -= salesOrder.discount;

    QSqlDatabase db = db::database();
    db.transaction();
    QSqlQuery q(db);
    q.prepare("insert into stock_updates"
              " ( type, datetime, party_name, party_address, total, discount, total_payment, total_paid, total_cost, total_profit)"
              " values"
              " (:type,:datetime,:party_name,:party_address,:total,:discount,:total_payment,:total_paid,:total_cost,:total_profit)");
    q.bindValue(":type", salesOrder.SalesOrder);
    q.bindValue(":datetime", salesOrder.dateTime);
    q.bindValue(":party_name", salesOrder.partyName);
    q.bindValue(":party_address", salesOrder.partyAddress);
    q.bindValue(":total", salesOrder.total);
    q.bindValue(":discount", salesOrder.discount);
    q.bindValue(":total_payment", salesOrder.totalPayment);
    q.bindValue(":total_paid", salesOrder.totalPaid);
    q.bindValue(":total_cost", salesOrder.totalCost);
    q.bindValue(":total_profit", salesOrder.totalProfit);
    if (!DB_EXEC(q)) {
        db.rollback();
        return false;
    }

    salesOrder.id = q.lastInsertId().toInt();
    for (const StockUpdateDetail &item: model->items) {
        Product* p = &editor->productsByIds[item.productId];
        if (p->type == Product::Stocked) {
            q.prepare("update products set stock=:stock where id=:id");
            q.bindValue(":stock", p->stock - item.quantity);
            q.bindValue(":id", p->id);
            if (!DB_EXEC(q)) {
                db.rollback();
                return false;
            }
        }

        q.prepare("insert into stock_update_details"
                  " ( update_id, product_id, product_name, product_uom, quantity, cost, price, subtotal_cost, subtotal_price)"
                  " values"
                  " (:update_id,:product_id,:product_name,:product_uom,:quantity,:cost,:price,:subtotal_cost,:subtotal_price)");
        q.bindValue(":update_id", salesOrder.id);
        q.bindValue(":product_id", item.productId);
        q.bindValue(":product_name", item.productName);
        q.bindValue(":product_uom", item.productUom);
        q.bindValue(":quantity", item.quantity);
        q.bindValue(":cost", item.cost);
        q.bindValue(":price", item.price);
        q.bindValue(":subtotal_cost", item.subtotalCost);
        q.bindValue(":subtotal_price", item.subtotalPrice);

        if (!DB_EXEC(q)) {
            db.rollback();
            return false;
        }
    }

    db.commit();
    lockUi();
    QMessageBox::information(this, "Informasi", "Rekaman penjualan telah disimpan.");

    return true;
}

void SalesOrderEditor::accept()
{
    if (!save())
        return;

    QDialog::accept();
}

void SalesOrderEditor::onModelChanged()
{
    double total = 0;
    for (const StockUpdateDetail &item: model->items) {
        total += item.subtotalPrice;
    }
    ui->totalEdit->setText(locale().toString(total, 'f', 0));
    updateTotal();
}

void SalesOrderEditor::updateInfoLabel()
{
    ui->recordInfoLabel->setText(QString("Menampilkan %1 item.").arg(locale().toString(model->rowCount())));
}

void SalesOrderEditor::updateTotal()
{
    double total = locale().toDouble(ui->totalEdit->text());
    double discount = locale().toDouble(ui->discountEdit->text());
    double grandTotal = total - discount;

    QString totalText = locale().toString(grandTotal, 'f', 0);

    ui->totalPaymentEdit->setText(totalText);
    ui->totalPaidEdit->setText(totalText);

    updateExchange();
}

void SalesOrderEditor::updateExchange()
{
    double totalPayment = locale().toDouble(ui->totalPaymentEdit->text());
    double totalPaid = locale().toDouble(ui->totalPaidEdit->text());
    double exchange = 0.;
    if (totalPaid != 0. && totalPaid > totalPayment)
        exchange = totalPaid - totalPayment;

    ui->exchangeEdit->setText(locale().toString(abs(exchange), 'f', 0));
    ui->totalPaidEdit->setStyleSheet("");
    if (totalPaid != 0. && totalPaid < totalPayment) {
        ui->totalPaidEdit->setStyleSheet("color:red; border: 1px solid red;");
    }
}

void SalesOrderEditor::load(const StockUpdate &order)
{
    lockUi();

    setWindowTitle("Lihat Penjualan");
    salesOrder = order;
    model->refresh(salesOrder.id);

    ui->customerNameEdit->setText(salesOrder.partyName);
    ui->customerAddressEdit->setText(salesOrder.partyAddress);
    ui->dateTimeEdit->setDateTime(salesOrder.dateTime);
    ui->idEdit->setText(salesOrder.salesOrderCode());
    ui->totalEdit->setText(locale().toString(salesOrder.total, 'f', 0));
    ui->discountEdit->setText(locale().toString(salesOrder.discount, 'f', 0));
    ui->totalPaymentEdit->setText(locale().toString(salesOrder.totalPayment, 'f', 0));
    ui->totalPaidEdit->setText(locale().toString(salesOrder.totalPaid, 'f', 0));
    ui->exchangeEdit->setText(locale().toString(salesOrder.exchange, 'f', 0));

    updateInfoLabel();
}

void SalesOrderEditor::lockUi()
{
    editor->setEnabled(false);
    model->readOnly = true;
    ui->customerNameEdit->setReadOnly(true);
    ui->customerAddressEdit->setReadOnly(true);
    ui->dateTimeEdit->setReadOnly(true);
    ui->totalPaidEdit->setReadOnly(true);
    ui->discountEdit->setReadOnly(true);
    ui->saveButton->setEnabled(false);

    editor->setVisible(false);
    ui->saveButton->setVisible(false);
    saveAction->setEnabled(false);
    removeAction->setEnabled(false);

    ui->shortcutLabel->setText("F12 = Cetak");
}

void SalesOrderEditor::saveAndPrint(bool showPreview)
{
    if (salesOrder.id == 0 && !save()) {
        return;
    }

    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.beginGroup("Print");
    int invoicePaperOption = settings.value("sales_invoice_papaer_size", 0).toInt();
    setProperty("sales_invoice_papaer_size", invoicePaperOption);
    settings.endGroup();

    QPrinter printer(QPrinter::ScreenResolution);
    printer.setColorMode(QPrinter::GrayScale);
    if (invoicePaperOption == 0) {
        printer.setPageSize(QPageSize(QSizeF(200, 140), QPageSize::Millimeter));
        printer.setPageOrientation(QPageLayout::Portrait);
        printer.setFullPage(false);
        printer.setPageMargins(QMarginsF(10, 10, 10, 10), QPageLayout::Millimeter);
    }
    else if (invoicePaperOption == 1) {
        printer.setPageSize(QPageSize(QSizeF(48, 300), QPageSize::Millimeter));
        printer.setPageOrientation(QPageLayout::Portrait);
        printer.setFullPage(false);
        printer.setPageMargins(QMarginsF(3, 3, 3, 3), QPageLayout::Millimeter);
    }
    else if (invoicePaperOption == 2) {
        printer.setPageSize(QPageSize(QSizeF(70, 300), QPageSize::Millimeter));
        printer.setPageOrientation(QPageLayout::Portrait);
        printer.setFullPage(false);
        printer.setPageMargins(QMarginsF(3, 3, 3, 3), QPageLayout::Millimeter);
    }

    if (showPreview) {
        QPrintPreviewDialog dialog(&printer, this, Qt::WindowCloseButtonHint | Qt::WindowMinMaxButtonsHint);
        connect(&dialog, SIGNAL(paintRequested(QPrinter*)), SLOT(printReceipt(QPrinter*)));
        dialog.setWindowTitle("Cetak Kwitansi");
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
    }
    else {
        printReceipt(&printer);
    }

}

void SalesOrderEditor::printReceipt(QPrinter *printer)
{
    QSettings settings("settings.ini", QSettings::IniFormat);

    int paperOption = property("sales_invoice_papaer_size").toInt();
    QString reportTemplate = paperOption == 0 ? "receipt-medium.html" : "receipt-small.html";
    QFile file(reportTemplate);
    file.open(QFile::ReadOnly | QFile::Text);

    QString html = QString::fromUtf8(file.readAll());
    settings.beginGroup("CompanyInfo");
    html.replace("{STORE_NAME}", settings.value("store_name").toString());
    html.replace("{STORE_ADDRESS}", settings.value("store_address").toString());
    settings.endGroup();
    html.replace("{ORDER_ID}", salesOrder.salesOrderCode());
    html.replace("{ORDER_DATE}", locale().toString(salesOrder.dateTime, "dd-MM-yyyy HH:mm"));
    html.replace("{CUSTOMER_NAME}", salesOrder.partyName);
    html.replace("{CUSTOMER_ADDRESS}", salesOrder.partyAddress);
    html.replace("{TOTAL}", locale().toString(salesOrder.total, 'f', 0));
    html.replace("{DISCOUNT}", locale().toString(salesOrder.discount, 'f', 0));
    html.replace("{TOTAL_PAYMENT}", locale().toString(salesOrder.totalPayment, 'f', 0));
    html.replace("{TOTAL_PAID}", locale().toString(salesOrder.totalPaid, 'f', 0));
    html.replace("{CASH_CHANGE}", locale().toString(abs(salesOrder.exchange), 'f', 0));
    QString items;
    int num = 0;
    for (StockUpdateDetail d: model->items) {
        num++;
        if (paperOption == 0) {
            items.append(QString(
                "<tr>"
                "<td align=\"center\">%1</td>"
                "<td>%2</td>"
                "<td align=\"center\">%3</td>"
                "<td align=\"center\">%4</td>"
                "<td align=\"right\">%5</td>"
                "<td align=\"right\">%6</td>"
                "</tr>")
            .arg(
                locale().toString(num),
                d.productName,
                locale().toString(d.quantity),
                d.productUom,
                locale().toString(d.price, 'f', 0),
                locale().toString(d.subtotalPrice, 'f', 0)
            ));
        }
        else if (paperOption == 1 || paperOption == 2) {
            items.append(QString(
                "<tr>"
                "<td>%1</td>"
                "<td align=\"center\">%2 %3</td>"
                "<td align=\"right\">%4</td>"
                "<td align=\"right\">%5</td>"
                "</tr>")
            .arg(
                d.productName,
                locale().toString(d.quantity),
                d.productUom,
                locale().toString(d.price, 'f', 0),
                locale().toString(d.subtotalPrice, 'f', 0)
            ));
        }
    }
    html.replace("{ITEMS}", items);

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

    printer->setDocName("NOTA #" + salesOrder.salesOrderCode());
    doc.print(printer);
}

void SalesOrderEditor::focusToDiscount()
{
    ui->discountEdit->setFocus();
    ui->discountEdit->selectAll();
}

void SalesOrderEditor::focusToPayment()
{
    ui->totalPaidEdit->setFocus();
    ui->totalPaidEdit->selectAll();
}
