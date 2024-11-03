#include "usereditor.h"
#include "ui_usereditor.h"
#include "db/db.h"
#include "common.h"
#include "entity/product.h"
#include "widgets/application.h"

#include <QMessageBox>

UserEditor::UserEditor(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::UserEditor)
{
    ui->setupUi(this);

    QMap<User::Role, QString>::iterator i;
    for (i = User::roleNames.begin(); i != User::roleNames.end(); ++i) {
        ui->roleComboBox->addItem(i.value(), i.key());
    }
    ui->roleComboBox->setCurrentIndex(ui->roleComboBox->findData(Product::Stocked));

    connect(ui->saveButton, SIGNAL(clicked()), SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(clicked()), SLOT(reject()));

    ui->saveButton->setIcon(qApp->qtAwesome()->icon("fa-solid fa-check"));
    ui->cancelButton->setIcon(qApp->qtAwesome()->icon("fa-solid fa-xmark"));
}

UserEditor::~UserEditor()
{
    delete ui;
}

void UserEditor::edit(const User &pItem)
{
    item = pItem;

    ui->fullNameEdit->setText(item.fullName);
    ui->usernameEdit->setText(item.username);
    ui->roleComboBox->setCurrentIndex(ui->roleComboBox->findData(item.role));
    ui->activeCheckBox->setChecked(item.active);

    User* currentUser = qApp->currentUser();

    if (item.username == "admin" || item.id == currentUser->id) {
        ui->usernameEdit->setReadOnly(true);
        ui->roleComboBox->setEnabled(false);
        ui->passwordEdit->setEnabled(false);
        ui->activeCheckBox->setEnabled(false);
    }

    setWindowTitle(QString("Ubah %1").arg(item.username));
}

void UserEditor::accept()
{
    item.username = ui->usernameEdit->text().trimmed();
    item.fullName = ui->fullNameEdit->text().trimmed();
    item.password = ui->passwordEdit->text();
    item.role = ui->roleComboBox->currentData().toInt();
    item.active = ui->activeCheckBox->isChecked();

    if (item.username.isEmpty()) {
        QMessageBox::warning(this, "Peringatan", "ID pengguna harus diisi.");
        ui->usernameEdit->setFocus();
        return;
    }

    QSqlQuery q(db::database());
    // Cek duplikat nama pengguna
    if (item.id == 0) {
        q.prepare("select count(0) from users where username=:username");
    }
    else {
        q.prepare("select count(0) from users where username=:username and id<>:id");
        q.bindValue(":id", item.id);
    }
    q.bindValue(":username", item.username);
    if (!DB_EXEC(q)) return;
    q.next();
    if (q.value(0).toInt() > 0) {
        QMessageBox::warning(this, "Peringatan", QString("Nama pengguna %1 sudah digunakan, silahkan gunakan nama lain.").arg(item.username));
        ui->usernameEdit->setFocus();
        ui->usernameEdit->selectAll();
        return;
    }

    if (item.fullName.isEmpty()) {
        QMessageBox::warning(this, "Peringatan", "Nama lengkap harus diisi.");
        ui->fullNameEdit->setFocus();
        return;
    }

    if (item.id == 0) {
        if (item.password.isEmpty()) {
            QMessageBox::warning(this, "Peringatan", "Kata sandi harus diisi.");
            ui->passwordEdit->setFocus();
            return;
        }
        q.prepare("insert into users"
                  " ( username, fullname, password, role, active) values"
                  " (:username,:fullname,:password,:role,:active)");
    }
    else {
        QString sql = "update users set"
                      " username=:username,"
                      " fullname=:fullname,"
                      " role=:role,"
                      " active=:active";
        if (!item.password.isEmpty()) {
            sql.append(",password=:password");
        }
        sql.append(" where id=:id");
        q.prepare(sql);
        q.bindValue(":id", item.id);
    }
    q.bindValue(":username", item.username);
    q.bindValue(":fullname", item.fullName);
    q.bindValue(":role", item.role);
    q.bindValue(":active", item.active);
    if (!item.password.isEmpty()) {
        q.bindValue(":password", encryptPassword(item.password));
    }
    DB_EXEC(q);

    if (!item.id) {
        item.id = q.lastInsertId().toInt();
    }

    QDialog::accept();
}
