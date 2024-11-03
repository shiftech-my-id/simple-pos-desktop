#include "purchaseordereditor.h"
#include "ui_purchaseordereditor.h"
#include "gui/purchaseordereditormodel.h"
#include "purchaseorderitemeditor.h"
#include "db/db.h"
#include "common.h"

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
#include <QDebug>

PurchaseOrderEditor::PurchaseOrderEditor(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint | Qt::WindowMinMaxButtonsHint),
    ui(new Ui::PurchaseOrderEditor),
    model(new PurchaseOrderEditorModel(this))
{
    ui->setupUi(this);

    editor = new PurchaseOrderItemEditor;
    connect(editor, &PurchaseOrderItemEditor::itemAdded, model, &PurchaseOrderEditorModel::addItem);

    editor->adjustSize();
    ui->itemsLayout->insertWidget(0, editor);

    ui->tableView->setModel(model);
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());

    connect(ui->saveButton, SIGNAL(clicked()), SLOT(accept()));
    connect(ui->printButton, SIGNAL(clicked()), SLOT(saveAndPrint()));
    connect(ui->cancelButton, SIGNAL(clicked()), SLOT(reject()));
    connect(model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), SLOT(onModelChanged()));
    connect(model, SIGNAL(rowsRemoved(QModelIndex,int,int)), SLOT(onModelChanged()));
    connect(model, SIGNAL(rowsInserted(QModelIndex,int,int)), SLOT(onModelChanged()));
    connect(model, SIGNAL(rowsRemoved(QModelIndex,int,int)), SLOT(updateInfoLabel()));
    connect(model, SIGNAL(rowsInserted(QModelIndex,int,int)), SLOT(updateInfoLabel()));

    removeAction = new QAction(this);
    addAction(removeAction);
    removeAction->setShortcut(Qt::Key_Delete);
    connect(removeAction, &QAction::triggered, this, &PurchaseOrderEditor::remove);

    saveAction = new QAction(this);
    addAction(saveAction);
    saveAction->setShortcut(Qt::Key_F11);
    connect(saveAction, &QAction::triggered, this, &PurchaseOrderEditor::save);

    QAction* printAction = new QAction(this);
    addAction(printAction);
    printAction->setShortcut(Qt::Key_F12);
    connect(printAction, &QAction::triggered, this, &PurchaseOrderEditor::saveAndPrint);

    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
}

PurchaseOrderEditor::~PurchaseOrderEditor()
{
    delete ui;
}

void PurchaseOrderEditor::reject()
{
    if (purchaseOrder.id == 0) {
        if (msgBoxQuestion(this, "Konfimasi", QString("Batalkan transaksi pembelian?")) != QMessageBox::Yes) {
            return;
        }
    }

    return QDialog::reject();
}

void PurchaseOrderEditor::fixNumber(QObject *sender)
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

void PurchaseOrderEditor::add()
{
    PurchaseOrderItemEditor editor(this);
    connect(&editor, SIGNAL(itemAdded(StockUpdateDetail)), model, SLOT(addItem(StockUpdateDetail)));
    editor.exec();
}

void PurchaseOrderEditor::remove()
{
    if (purchaseOrder.id) {
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

bool PurchaseOrderEditor::save()
{
    if (msgBoxQuestion(this, "Konfirmasi", "Simpan transaksi pembelian?") != QMessageBox::Yes) {
        return false;
    }

    if (purchaseOrder.id) {
        // PENGEDITAN TIDAK DIPERBOLEHKAN
        return true;
    }

    if (model->rowCount() == 0) {
        QMessageBox::information(this, "Informasi", "Belum ada item yang ditambahkan.");
        return false;
    }

    purchaseOrder.partyName = ui->supplierNameEdit->text().trimmed();
    purchaseOrder.partyAddress = ui->supplierAddressEdit->text().trimmed();
    purchaseOrder.dateTime = ui->dateTimeEdit->dateTime();
    purchaseOrder.total = locale().toDouble(ui->totalEdit->text());
    purchaseOrder.discount = 0;
    purchaseOrder.totalPayment = purchaseOrder.total;
    purchaseOrder.totalPaid = purchaseOrder.total;

    purchaseOrder.totalCost = 0.;
    purchaseOrder.totalProfit = 0.;
    for (StockUpdateDetail &item: model->items) {
        purchaseOrder.totalCost += item.subtotalCost;
        purchaseOrder.totalProfit += (item.subtotalPrice - item.subtotalCost);
    }
    purchaseOrder.totalProfit -= purchaseOrder.discount;

    QSqlDatabase db = db::database();
    db.transaction();
    QSqlQuery q(db);
    q.prepare("insert into stock_updates"
              " ( type, datetime, party_name, party_address, total, discount, total_payment, total_paid, total_cost, total_profit)"
              " values"
              " (:type,:datetime,:party_name,:party_address,:total,:discount,:total_payment,:total_paid,:total_cost,:total_profit)");
    q.bindValue(":type", purchaseOrder.PurchaseOrder);
    q.bindValue(":datetime", purchaseOrder.dateTime);
    q.bindValue(":party_name", purchaseOrder.partyName);
    q.bindValue(":party_address", purchaseOrder.partyAddress);
    q.bindValue(":total", purchaseOrder.total);
    q.bindValue(":discount", purchaseOrder.discount);
    q.bindValue(":total_payment", purchaseOrder.totalPayment);
    q.bindValue(":total_paid", purchaseOrder.totalPaid);
    q.bindValue(":total_cost", purchaseOrder.totalCost);
    q.bindValue(":total_profit", purchaseOrder.totalProfit);
    if (!DB_EXEC(q)) {
        db.rollback();
        return false;
    }

    purchaseOrder.id = q.lastInsertId().toInt();
    for (StockUpdateDetail &item: model->items) {
        Product* p = &editor->productsByIds[item.productId];
        q.prepare("update products set cost=:cost, price=:price where id=:id");
        q.bindValue(":cost", item.cost);
        q.bindValue(":price", item.price);
        q.bindValue(":id", p->id);
        if (!DB_EXEC(q)) {
            db.rollback();
            return false;
        }

        if (p->type == Product::Stocked) {
            q.prepare("update products set stock=:stock where id=:id");
            q.bindValue(":stock", p->stock + item.quantity);
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
        q.bindValue(":update_id", purchaseOrder.id);
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

    QMessageBox::information(this, "Informasi", "Rekaman pembelian telah disimpan.");

    return true;
}

void PurchaseOrderEditor::accept()
{
    if (!save())
        return;

    QDialog::accept();
}

void PurchaseOrderEditor::onModelChanged()
{
    double total = 0;
    for (const StockUpdateDetail &item: model->items) {
        total += item.subtotalCost;
    }
    ui->totalEdit->setText(locale().toString(total, 'f', 0));
}

void PurchaseOrderEditor::updateInfoLabel()
{
    ui->recordInfoLabel->setText(QString("Menampilkan %1 item.").arg(locale().toString(model->rowCount())));
}

void PurchaseOrderEditor::load(const StockUpdate &order)
{
    lockUi();

    setWindowTitle("Lihat Pembelian");
    purchaseOrder = order;
    model->refresh(purchaseOrder.id);

    ui->supplierNameEdit->setText(purchaseOrder.partyName);
    ui->supplierAddressEdit->setText(purchaseOrder.partyAddress);
    ui->dateTimeEdit->setDateTime(purchaseOrder.dateTime);
    ui->idEdit->setText(purchaseOrder.purchaseOrderCode());
    ui->totalEdit->setText(locale().toString(purchaseOrder.totalCost, 'f', 0));

    updateInfoLabel();
}

void PurchaseOrderEditor::lockUi()
{
    editor->setEnabled(false);
    model->readOnly = true;
    ui->supplierNameEdit->setReadOnly(true);
    ui->supplierAddressEdit->setReadOnly(true);
    ui->dateTimeEdit->setReadOnly(true);
    ui->saveButton->setEnabled(false);
    editor->setVisible(false);
    ui->saveButton->setVisible(false);
    saveAction->setEnabled(false);
    removeAction->setEnabled(false);
    ui->shortcutLabel->setText("F12 = Cetak");
}

void PurchaseOrderEditor::saveAndPrint(bool showPreview)
{
    if (purchaseOrder.id == 0 && !save()) {
        return;
    }

    QPrinter printer(QPrinter::ScreenResolution);
    printer.setColorMode(QPrinter::GrayScale);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageOrientation(QPageLayout::Portrait);
    printer.setFullPage(false);
    printer.setPageMargins(QMarginsF(20, 20, 20, 20), QPageLayout::Millimeter);

    if (showPreview) {
        QPrintPreviewDialog dialog(&printer, this, Qt::WindowCloseButtonHint | Qt::WindowMinMaxButtonsHint);
        connect(&dialog, SIGNAL(paintRequested(QPrinter*)), SLOT(printReceipt(QPrinter*)));
        dialog.setWindowTitle("Cetak Faktur Pembelian");
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

void PurchaseOrderEditor::printReceipt(QPrinter *printer)
{
    QSettings settings("settings.ini", QSettings::IniFormat);

    QString reportTemplate = "po-invoice.html";
    QFile file(reportTemplate);
    file.open(QFile::ReadOnly | QFile::Text);

    QString html = QString::fromUtf8(file.readAll());
    settings.beginGroup("CompanyInfo");
    html.replace("{STORE_NAME}", settings.value("store_name").toString());
    html.replace("{STORE_ADDRESS}", settings.value("store_address").toString());
    settings.endGroup();
    html.replace("{ORDER_ID}", purchaseOrder.purchaseOrderCode());
    html.replace("{ORDER_DATE}", locale().toString(purchaseOrder.dateTime, "dd-MM-yyyy HH:mm"));
    html.replace("{SUPPLIER_NAME}", purchaseOrder.partyName);
    html.replace("{SUPPLIER_ADDRESS}", purchaseOrder.partyAddress);
    html.replace("{TOTAL}", locale().toString(purchaseOrder.totalCost, 'f', 0));
    QString items;
    int num = 0;
    for (StockUpdateDetail d: model->items) {
        num++;
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
            locale().toString(d.cost, 'f', 0),
            locale().toString(d.subtotalCost, 'f', 0)
        ));
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

    printer->setDocName("NOTA #" + purchaseOrder.purchaseOrderCode());
    doc.print(printer);
}
