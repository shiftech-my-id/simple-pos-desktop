#include "changepassworddialog.h"
#include "ui_changepassworddialog.h"
#include <QMessageBox>
#include "application.h"
#include "db/db.h"
#include "common.h"
#include "entity/user.h"

ChangePasswordDialog::ChangePasswordDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChangePasswordDialog)
{
    ui->setupUi(this);
    setWindowTitle("Ganti Kata Sandi");

    connect(ui->saveButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    adjustSize();
    setFixedSize(size());
}

ChangePasswordDialog::~ChangePasswordDialog()
{
    delete ui;
}

void ChangePasswordDialog::accept()
{
    QString currentPassword = ui->currentPasswordEdit->text().trimmed();
    QString newPassword = ui->newPasswordEdit->text().trimmed();
    QString passConfirmation = ui->confirmPasswordEdit->text().trimmed();

    if (currentPassword.isEmpty()) {
        ui->currentPasswordEdit->setFocus();
        QMessageBox::warning(this, "Peringatan", "Kata sandi harus diisi.");
        return;
    }

    if (newPassword.isEmpty()) {
        ui->newPasswordEdit->setFocus();
        QMessageBox::warning(this, "Peringatan", "Masukkan kata sandi baru.");
        return;
    }

    if (passConfirmation.isEmpty()) {
        ui->confirmPasswordEdit->setFocus();
        QMessageBox::warning(this, "Peringatan", "Konfirmasi kata sandi baru.");
        return;
    }

    if (newPassword != passConfirmation) {
        ui->confirmPasswordEdit->setFocus();
        QMessageBox::warning(this, "Peringatan", "Kata sandi yang anda konfirmasi salah.");
        return;
    }

    User* user = qApp->currentUser();

    QSqlQuery q(db::database());
    q.prepare("select password from users where id=:id");
    q.bindValue(":id", user->id);
    DB_EXEC(q);

    if (!q.next()) {
        ui->currentPasswordEdit->setFocus();
        QMessageBox::warning(this, "Peringatan", "ID Pengguna tidak ditemukan.");
        return;
    }

    if (q.value("password").toString() != encryptPassword(currentPassword)) {
        ui->currentPasswordEdit->setFocus();
        ui->currentPasswordEdit->selectAll();
        QMessageBox::warning(this, "Peringatan", "Kata sandi yang anda masukkan salah.");
        return;
    }

    q.clear();
    q.prepare("update users set password=:password where id=:id");
    q.bindValue(":id", user->id);
    q.bindValue(":password", encryptPassword(newPassword));
    DB_EXEC(q);

    QDialog::accept();

    QMessageBox::information(this, "Informasi", "Kata sandi berhasil diganti.");
}
