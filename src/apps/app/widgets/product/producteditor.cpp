#include "producteditor.h"
#include "ui_producteditor.h"
#include "productcategoryeditor.h"
#include "db/db.h"
#include "widgets/application.h"
#include "widgets/supplier/suppliereditor.h"
#include "gui/productcategorymodel.h"
#include "gui/productcategoryproxymodel.h"
#include "gui/suppliermodel.h"
#include "gui/supplierproxymodel.h"

#include <QMessageBox>
#include <QAbstractItemView>

ProductEditor::ProductEditor(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::ProductEditor),
    categoryModel(qApp->productCategoryModel()),
    categoryProxyModel(new ProductCategoryProxyModel(this)),
    supplierModel(qApp->supplierModel()),
    supplierProxyModel(new SupplierProxyModel(this))
{
    ui->setupUi(this);

    categoryProxyModel->setSourceModel(categoryModel);
    categoryProxyModel->sort(0);

    ui->categoryComboBox->setModel(categoryProxyModel);
    ui->categoryComboBox->setModelColumn(0);
    ui->categoryComboBox->view()->setAlternatingRowColors(true);

    supplierProxyModel->setSourceModel(supplierModel);
    supplierProxyModel->sort(0);
    ui->supplierComboBox->setModel(supplierProxyModel);
    ui->supplierComboBox->setModelColumn(0);
    ui->supplierComboBox->view()->setAlternatingRowColors(true);

    QMap<Product::Type, QString>::iterator i;
    for (i = Product::typeNames.begin(); i != Product::typeNames.end(); ++i) {
        ui->typeComboBox->addItem(i.value(), i.key());
    }
    ui->typeComboBox->setCurrentIndex(ui->typeComboBox->findData(Product::Stocked));

    QSqlQuery q(db::database());
    q.prepare("select uom from products group by uom order by uom asc");
    DB_EXEC(q);
    while (q.next())
        ui->uomComboBox->addItem(q.value(0).toString());
    ui->uomComboBox->setCurrentIndex(-1);

    connect(ui->saveButton, SIGNAL(clicked()), SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(clicked()), SLOT(reject()));
    connect(ui->addCategoryButton, SIGNAL(clicked()), SLOT(addCategory()));
    connect(ui->addSupplierButton, SIGNAL(clicked()), SLOT(addSupplier()));

    ui->saveButton->setIcon(qApp->qtAwesome()->icon("fa-solid fa-check"));
    ui->cancelButton->setIcon(qApp->qtAwesome()->icon("fa-solid fa-xmark"));
    ui->addCategoryButton->setIcon(qApp->qtAwesome()->icon("fa-solid fa-plus"));
    ui->addSupplierButton->setIcon(qApp->qtAwesome()->icon("fa-solid fa-plus"));
}

ProductEditor::~ProductEditor()
{
    delete ui;
}

void ProductEditor::edit(const Product &pItem)
{
    item = pItem;
    ui->nameEdit->setText(item.name);
    ui->descriptionEdit->setText(item.description);
    ui->barcodeEdit->setText(item.barcode);
    ui->typeComboBox->setCurrentIndex(ui->typeComboBox->findData(item.type));
    ui->categoryComboBox->setCurrentIndex(ui->categoryComboBox->findData(item.categoryId));
    ui->supplierComboBox->setCurrentIndex(ui->supplierComboBox->findData(item.supplierId));
    ui->uomComboBox->lineEdit()->setText(item.uom);
    ui->activeCheckBox->setChecked(item.active);
    ui->stockSpinBox->setValue(item.stock);
    ui->costEdit->setValue(item.cost);
    ui->priceEdit->setValue(item.price);
    ui->nameEdit->setFocus();
    setWindowTitle(QString("Ubah %1").arg(item.name));
}

void ProductEditor::duplicate(const Product &item)
{
    edit(item);
    this->item.id = 0;
    ui->nameEdit->selectAll();
    ui->nameEdit->setFocus();
    setWindowTitle("Tambah Produk");
}

void ProductEditor::accept()
{
    item.name = ui->nameEdit->text().trimmed();
    item.description = ui->descriptionEdit->text().trimmed();
    item.barcode = ui->barcodeEdit->text().trimmed();
    item.uom = ui->uomComboBox->lineEdit()->text().trimmed();
    item.cost = ui->costEdit->value();
    item.price = ui->priceEdit->value();
    item.type = ui->typeComboBox->currentData().toInt();
    item.stock = ui->stockSpinBox->value();
    item.active = ui->activeCheckBox->isChecked();
    item.categoryId = categoryProxyModel->index(ui->categoryComboBox->currentIndex(), 0).data(Qt::UserRole).toInt();
    item.supplierId = supplierProxyModel->index(ui->supplierComboBox->currentIndex(), 0).data(Qt::UserRole).toInt();

    if (item.name.isEmpty()) {
        QMessageBox::warning(this, "Peringatan", "Nama produk harus diisi.");
        ui->nameEdit->setFocus();
        return;
    }

    QSqlQuery q(db::database());
    // Cek duplikat nama produk
    if (item.id == 0) {
        q.prepare("select count(0) from products where name=:name");
    }
    else {
        q.prepare("select count(0) from products where name=:name and id<>:id");
        q.bindValue(":id", item.id);
    }
    q.bindValue(":name", item.name);
    if (!DB_EXEC(q)) return;
    q.next();
    if (q.value(0).toInt() > 0) {
        QMessageBox::warning(this, "Peringatan",
                             QString("Nama produk %1 sudah digunakan, silahkan gunakan nama lain.").arg(item.name));
        ui->nameEdit->setFocus();
        ui->nameEdit->selectAll();
        return;
    }

    // Cek duplikat jika barcode diisi
    if (item.barcode.length() > 0) {
        if (item.id == 0) {
            q.prepare("select count(0) from products where barcode=:barcode");
        }
        else {
            q.prepare("select count(0) from products where barcode=:barcode and id<>:id");
            q.bindValue(":id", item.id);
        }
        q.bindValue(":barcode", item.barcode);
        if (!DB_EXEC(q)) return;
        q.next();
        if (q.value(0).toInt() > 0) {
            QMessageBox::warning(this, "Peringatan",
                                 QString("Barcode %1 sudah digunakan.").arg(item.barcode));
            ui->barcodeEdit->setFocus();
            ui->barcodeEdit->selectAll();
            return;
        }
    }


    if (item.id == 0) {
        q.prepare("insert into products"
                  " ( name, description, barcode, uom, cost, price, type, stock, active, category_id, supplier_id) values"
                  " (:name,:description,:barcode,:uom,:cost,:price,:type,:stock,:active,:category_id,:supplier_id)");
    }
    else {
        q.prepare("update products set"
                  " name=:name,"
                  " description=:description,"
                  " barcode=:barcode,"
                  " uom=:uom,"
                  " cost=:cost,"
                  " price=:price,"
                  " type=:type,"
                  " stock=:stock,"
                  " active=:active,"
                  " category_id=:category_id,"
                  " supplier_id=:supplier_id"
                  " where id=:id");
        q.bindValue(":id", item.id);
    }
    q.bindValue(":name", item.name);
    q.bindValue(":description", item.description);
    q.bindValue(":barcode", item.barcode);
    q.bindValue(":uom", item.uom);
    q.bindValue(":cost", item.cost);
    q.bindValue(":price", item.price);
    q.bindValue(":type", item.type);
    q.bindValue(":category_id", item.categoryId == 0 ? QVariant() : item.categoryId);
    q.bindValue(":supplier_id", item.supplierId == 0 ? QVariant() : item.supplierId);
    q.bindValue(":stock", item.stock);
    q.bindValue(":active", item.active);
    DB_EXEC(q);

    if (!item.id) {
        item.id = q.lastInsertId().toInt();
    }

    QDialog::accept();
}

void ProductEditor::addCategory()
{
    ProductCategoryEditor editor;
    if (!editor.exec())
        return;

    const QString name = editor.name();
    ui->categoryComboBox->setCurrentIndex(ui->categoryComboBox->findText(name));
}

void ProductEditor::addSupplier()
{
    SupplierEditor editor;
    if (!editor.exec())
        return;

    const QString name = editor.name();
    ui->supplierComboBox->setCurrentIndex(ui->supplierComboBox->findText(name));
}
