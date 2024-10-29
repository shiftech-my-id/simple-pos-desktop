#include "logindialog.h"
#include "ui_logindialog.h"
#include "global.h"
#include "db.h"
#include "common.h"
#include "application.h"

#include <QSettings>
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    setWindowTitle("Masuk");

    ui->appInfoLabel->setText(QString("%1 %2").arg(APP_DISPLAY_NAME, APP_VERSION_STR));

    QSettings settings(SETTINGS_PATH, QSettings::IniFormat);
    settings.beginGroup("CompanyInfo");
    ui->companyNameLabel->setText(settings.value("company_name", "My Store").toString());

    QString address = settings.value("company_address").toString();
    if (address.isEmpty()) {
        ui->companyAddressLabel->hide();
    }
    else {
        ui->companyAddressLabel->setText(address);
    }
    settings.endGroup();

    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    ui->okButton->setIcon(qApp->qtAwesome()->icon("fa-solid fa-check"));
    ui->cancelButton->setIcon(qApp->qtAwesome()->icon("fa-solid fa-xmark"));

    adjustSize();
    setFixedSize(size());
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::accept()
{
    QString username = ui->usernameEdit->text().trimmed();
    QString password = ui->passwordEdit->text().trimmed();

    if (username.isEmpty()) {
        ui->usernameEdit->setFocus();
        QMessageBox::warning(this, "Peringatan", "ID Pengguna harus diisi.");
        return;
    }

    QSqlQuery q(QSqlDatabase::database());
    q.prepare("select * from users where username=:username");
    q.bindValue(":username", username);
    DB_EXEC(q);

    if (!q.next()) {
        ui->usernameEdit->setFocus();
        QMessageBox::warning(this, "Peringatan", "ID Pengguna tidak ditemukan.");
        return;
    }

    if (password.isEmpty()) {
        ui->passwordEdit->setFocus();
        QMessageBox::warning(this, "Peringatan", "Kata sandi harus diisi.");
        return;
    }

    if (!q.value("active").toBool()) {
        ui->usernameEdit->setFocus();
        QMessageBox::warning(this, "Peringatan", "Akun anda tidak aktif, silahkan hubungi administrator.");
        return;
    }

    if (q.value("password").toString() != encryptPassword(password)) {
        ui->passwordEdit->setFocus();
        ui->passwordEdit->clear();
        QMessageBox::warning(this, "Peringatan", "Kata sandi yang anda masukkan salah.");
        return;
    }

    login(q.value("id").toInt());
    QDialog::accept();
}

void LoginDialog::login(int id)
{
    QSqlQuery q(QSqlDatabase::database());
    q.prepare("select * from users where id=:id");
    q.bindValue(":id", id);
    DB_EXEC(q);
    q.next();

    QVariantMap user;
    user["id"] = q.value("id");
    user["username"] = q.value("username");
    qApp->setProperty("current_user", user);
}
