#include "productcategoryeditor.h"
#include "ui_productcategoryeditor.h"
#include "db.h"
#include <QMessageBox>

ProductCategoryEditor::ProductCategoryEditor(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::ProductCategoryEditor)
{
    ui->setupUi(this);

    connect(ui->saveButton, SIGNAL(clicked()), SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(clicked()), SLOT(reject()));

    setWindowTitle("Tambah Kategori");
}

ProductCategoryEditor::~ProductCategoryEditor()
{
    delete ui;
}

void ProductCategoryEditor::edit(const ProductCategory &cat)
{
    item = cat;
    ui->nameEdit->setText(item.name);
    setWindowTitle(QString("Ubah Kategori %1").arg(item.name));
}


void ProductCategoryEditor::accept()
{
    item.name = ui->nameEdit->text().trimmed();

    if (item.name.isEmpty()) {
        QMessageBox::warning(this, "Peringatan", "Nama kategori harus diisi.");
        ui->nameEdit->setFocus();
        return;
    }

    QSqlQuery q(db::database());
    // Cek duplikat nama kategori
    if (item.id == 0) {
        q.prepare("select count(0) from product_categories where name=:name");
    }
    else {
        q.prepare("select count(0) from product_categories where name=:name and id<>:id");
        q.bindValue(":id", item.id);
    }
    q.bindValue(":name", item.name);
    if (!DB_EXEC(q)) return;

    q.next();
    if (q.value(0).toInt() > 0) {
        QMessageBox::warning(this, "Peringatan",
                             QString("Nama kategori %1 sudah digunakan, silahkan gunakan nama lain.").arg(item.name));
        ui->nameEdit->setFocus();
        ui->nameEdit->selectAll();
        return;
    }

    if (item.id == 0) {
        q.prepare("insert into product_categories"
                  " ( name) values"
                  " (:name)");
    }
    else {
        q.prepare("update product_categories set"
                  " name=:name"
                  " where id=:id");
        q.bindValue(":id", item.id);
    }
    q.bindValue(":name", item.name);
    DB_EXEC(q);

    if (!item.id) {
        item.id = q.lastInsertId().toInt();
    }

    QDialog::accept();
}
