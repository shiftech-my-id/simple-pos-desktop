#include "producteditor.h"
#include "ui_producteditor.h"
#include "productcategoryeditor.h"
#include "db.h"
#include "application.h"

#include <QMessageBox>
#include "productcategorymodel.h"

ProductEditor::ProductEditor(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::ProductEditor)
{
    ui->setupUi(this);

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

    ui->saveButton->setIcon(qApp->qtAwesome()->icon("fa-solid fa-check"));
    ui->cancelButton->setIcon(qApp->qtAwesome()->icon("fa-solid fa-xmark"));
    ui->addCategoryButton->setIcon(qApp->qtAwesome()->icon("fa-solid fa-plus"));

    refreshCategories();
}

ProductEditor::~ProductEditor()
{
    delete ui;
}

void ProductEditor::edit(const Product &pItem)
{
    refreshCategories();

    item = pItem;
    ui->nameEdit->setText(item.name);
    ui->descriptionEdit->setText(item.description);
    ui->barcodeEdit->setText(item.barcode);
    ui->typeComboBox->setCurrentIndex(ui->typeComboBox->findData(item.type));
    ui->categoryComboBox->setCurrentIndex(ui->categoryComboBox->findData(item.categoryId));
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
    item.categoryId = ui->categoryComboBox->currentData().toInt();

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
                  " ( name, description, barcode, uom, cost, price, type, stock, active, category_id) values"
                  " (:name,:description,:barcode,:uom,:cost,:price,:type,:stock,:active,:category_id)");
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
                  " category_id=:category_id"
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
    q.bindValue(":stock", item.stock);
    q.bindValue(":active", item.active);
    DB_EXEC(q);

    if (!item.id) {
        item.id = q.lastInsertId().toInt();
    }

    QDialog::accept();
}

void ProductEditor::refreshCategories()
{
    ui->categoryComboBox->clear();
    QSqlQuery q(db::database());
    q.prepare("select id, name from product_categories order by name asc");
    DB_EXEC(q);
    while (q.next())
        ui->categoryComboBox->addItem(q.value("name").toString(), q.value("id").toInt());
    ui->categoryComboBox->setCurrentIndex(-1);
}

void ProductEditor::addCategory()
{
    ProductCategoryEditor editor;
    if (!editor.exec())
        return;

    refreshCategories();
}
