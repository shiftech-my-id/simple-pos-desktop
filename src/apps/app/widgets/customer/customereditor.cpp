#include "customereditor.h"
#include "ui_customereditor.h"
#include "db/db.h"
#include "widgets/application.h"

#include <QMessageBox>
#include <QAbstractItemView>

CustomerEditor::CustomerEditor(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::CustomerEditor)
{
    ui->setupUi(this);

    connect(ui->saveButton, SIGNAL(clicked()), SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(clicked()), SLOT(reject()));

    ui->saveButton->setIcon(qApp->qtAwesome()->icon("fa-solid fa-check"));
    ui->cancelButton->setIcon(qApp->qtAwesome()->icon("fa-solid fa-xmark"));
}

CustomerEditor::~CustomerEditor()
{
    delete ui;
}

void CustomerEditor::edit(const Customer &pItem)
{
    item = pItem;
    ui->nameEdit->setText(item.name);
    ui->addressEdit->setText(item.address);
    ui->phoneEdit->setText(item.phone);
    ui->activeCheckBox->setChecked(item.active);
    ui->balanceEdit->setValue(item.balance);
    ui->nameEdit->setFocus();
    setWindowTitle(QString("Ubah %1").arg(item.name));
}

void CustomerEditor::duplicate(const Customer &item)
{
    edit(item);
    this->item.id = 0;
    ui->nameEdit->selectAll();
    ui->nameEdit->setFocus();
    setWindowTitle("Tambah Pelanggan");
}

void CustomerEditor::accept()
{
    item.name = ui->nameEdit->text().trimmed();
    item.address = ui->addressEdit->text().trimmed();
    item.phone = ui->phoneEdit->text().trimmed();
    item.balance = ui->balanceEdit->value();
    item.active = ui->activeCheckBox->isChecked();

    if (item.name.isEmpty()) {
        QMessageBox::warning(this, "Peringatan", "Nama pelanggan harus diisi.");
        ui->nameEdit->setFocus();
        return;
    }

    QSqlQuery q(db::database());
    // Cek duplikat
    if (item.id == 0) {
        q.prepare("select count(0) from parties where type=0 and name=:name");
    }
    else {
        q.prepare("select count(0) from parties where type=0 and name=:name and id<>:id");
        q.bindValue(":id", item.id);
    }
    q.bindValue(":name", item.name);
    if (!DB_EXEC(q)) return;
    q.next();
    if (q.value(0).toInt() > 0) {
        QMessageBox::warning(this, "Peringatan", QString("Nama pelanggan %1 sudah digunakan, silahkan gunakan nama lain.").arg(item.name));
        ui->nameEdit->setFocus();
        ui->nameEdit->selectAll();
        return;
    }

    if (item.id == 0) {
        q.prepare("insert into parties"
                  " ( name, address, phone, balance, active, type) values"
                  " (:name,:address,:phone,:balance,:active, 0)");
    }
    else {
        q.prepare("update parties set"
                  " name=:name,"
                  " address=:address,"
                  " phone=:phone,"
                  " balance=:balance,"
                  " active=:active"
                  " where id=:id");
        q.bindValue(":id", item.id);
    }
    q.bindValue(":name", item.name);
    q.bindValue(":address", item.address);
    q.bindValue(":phone", item.phone);
    q.bindValue(":balance", item.balance);
    q.bindValue(":active", item.active);
    DB_EXEC(q);

    if (!item.id) {
        item.id = q.lastInsertId().toInt();
    }

    QDialog::accept();
}
