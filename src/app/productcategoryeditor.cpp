#include "productcategoryeditor.h"
#include "ui_productcategoryeditor.h"
#include "productcategorymodel.h"
#include "db.h"
#include "application.h"
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

    if (ProductCategoryModel::exists(item.name, item.id)) {
        QMessageBox::warning(this, "Peringatan", QString("Nama kategori %1 sudah digunakan, silahkan gunakan nama lain.").arg(item.name));
        ui->nameEdit->setFocus();
        ui->nameEdit->selectAll();
        return;
    }

    qApp->productCategoryModel()->save(item);

    QDialog::accept();
}

QString ProductCategoryEditor::name() const
{
    return ui->nameEdit->text().trimmed();
}
