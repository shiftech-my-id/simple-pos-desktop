#include "salesorderitemeditor.h"
#include "ui_salesorderitemeditor.h"
#include "widgets/product/producteditor.h"
#include "db/db.h"

#include <QAbstractItemView>
#include <QKeyEvent>
#include <QCompleter>
#include <QMessageBox>
#include <QListView>
#include "gui/productmodel.h"

SalesOrderItemEditor::SalesOrderItemEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SalesOrderItemEditor)
{
    ui->setupUi(this);

    ui->productComboBox->setInsertPolicy(QComboBox::NoInsert);
    ui->productComboBox->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
    ui->productComboBox->completer()->setCaseSensitivity(Qt::CaseInsensitive);
    ui->productComboBox->completer()->setFilterMode(Qt::MatchContains);
    ui->productComboBox->completer()->setCompletionMode(QCompleter::PopupCompletion);
    ui->productComboBox->completer()->popup()->setFont(ui->productComboBox->font());
    ui->productComboBox->lineEdit()->setPlaceholderText("Nama Produk / Barcode");
    QListView *view = static_cast<QListView *>(ui->productComboBox->view());
    view->setUniformItemSizes(true);
    view->setLayoutMode(QListView::Batched);

    refreshProducts();

    connect(ui->productComboBox, SIGNAL(currentIndexChanged(int)), SLOT(onProductChanged()));
    connect(ui->addButton, SIGNAL(clicked()), SLOT(addItem()));
    connect(ui->productComboBox->lineEdit(), SIGNAL(returnPressed()), SLOT(updateUi()));
    connect(ui->newProductButton, SIGNAL(clicked()), SLOT(addNewProduct()));

    ui->productComboBox->lineEdit()->installEventFilter(this);
    ui->productComboBox->installEventFilter(this);
    ui->quantitySpinBox->installEventFilter(this);
    ui->priceEdit->installEventFilter(this);
    ui->addButton->installEventFilter(this);
}

void SalesOrderItemEditor::refreshProducts()
{
    ui->productComboBox->clear();
    productsByIds.clear();

    QSqlQuery q(db::database());
    q.prepare("select * from products order by name asc");
    DB_EXEC(q);
    while (q.next()) {
        Product product;
        product.id = q.value("id").toInt();
        product.name = q.value("name").toString();
        product.uom = q.value("uom").toString();
        product.barcode = q.value("barcode").toString();
        product.cost = q.value("cost").toDouble();
        product.price = q.value("price").toDouble();
        product.type = q.value("type").toInt();
        product.stock = q.value("stock").toInt();
        product.categoryId = q.value("category_id").toInt();
        product.active = q.value("active").toBool();
        productsByIds.insert(product.id, product);
        ui->productComboBox->addItem(product.name, product.id);
    }
    ui->productComboBox->setCurrentIndex(-1);
}

SalesOrderItemEditor::~SalesOrderItemEditor()
{
    delete ui;
}

void SalesOrderItemEditor::addNewProduct()
{
    ProductEditor editor(this);
    if (!editor.exec()) {
        return;
    }
    Product product = editor.item;
    ui->productComboBox->addItem(product.name, product.id);
    productsByIds.insert(product.id, product);
    ui->productComboBox->setCurrentText(product.name);
    ui->productComboBox->setCurrentIndex(ui->productComboBox->findText(product.name));
    ui->productComboBox->lineEdit()->setFocus();
}

void SalesOrderItemEditor::onProductChanged()
{
    ui->quantitySpinBox->setValue(1);
    updateUi();
}

void SalesOrderItemEditor::updateUi()
{
    ui->addButton->setEnabled(true);

    QString productName = ui->productComboBox->lineEdit()->text().trimmed();
    int index = -1;
    // find by name or barcode
    if (productName.length() > 0) {
        for (int i = 0; i < ui->productComboBox->count(); i++) {
            Product p = productsByIds.value(ui->productComboBox->itemData(i).toInt());
            if (p.name.toLower() == productName.toLower()
                    || p.barcode.toLower() == productName.toLower()) {
                index = i;
                break;
            }
        }
    }

    if (index == -1) {
        ui->productComboBox->setCurrentIndex(-1);
        ui->productComboBox->lineEdit()->setText(productName);
        ui->addButton->setEnabled(false);
        if (sender() == ui->productComboBox->lineEdit()) {
            QMessageBox::warning(this, "Peringatan", "Produk tidak ditemukan.");
        }
        ui->productComboBox->lineEdit()->setFocus();
        return;
    }

    ui->productComboBox->setCurrentIndex(index);
    int id = ui->productComboBox->currentData().toInt();
    currentProduct = productsByIds.value(id);
    ui->uomLabel->setText(currentProduct.uom);
    ui->priceEdit->setValue(currentProduct.price);
}

void SalesOrderItemEditor::addItem()
{
    if (ui->productComboBox->currentIndex() == -1
            || ui->quantitySpinBox->value() == 0)
        return;

    StockUpdateDetail item;
    item.productId = currentProduct.id;
    item.productName = currentProduct.name;
    item.productUom = currentProduct.uom;
    item.cost = currentProduct.cost;

    // set sesuai yg diisi
    item.quantity = ui->quantitySpinBox->value();
    item.price = ui->priceEdit->value();

    item.subtotalCost = item.cost * item.quantity;
    item.subtotalPrice = item.price * item.quantity;

    ui->productComboBox->setCurrentIndex(-1);
    ui->productComboBox->setFocus();
    ui->priceEdit->setValue(0);
    ui->quantitySpinBox->setValue(1);
    ui->uomLabel->setText("");

    emit itemAdded(item);
}

bool SalesOrderItemEditor::eventFilter(QObject *o, QEvent* event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *k = static_cast<QKeyEvent*>(event);
        if (k->key() == Qt::Key_Return || k->key() == Qt::Key_Enter) {
            if (o == ui->productComboBox->lineEdit() || o == ui->productComboBox) {
                ui->quantitySpinBox->setFocus();
                ui->quantitySpinBox->selectAll();
            }
            else if (o == ui->quantitySpinBox) {
                ui->priceEdit->setFocus();
                ui->priceEdit->selectAll();
            }
            else if (o == ui->priceEdit) {
                ui->addButton->setFocus();
            }
            else if (o == ui->addButton) {
                addItem();
            }
        }
    }

    return QDialog::eventFilter(o, event);
}
