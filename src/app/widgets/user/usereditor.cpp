#include "usereditor.h"
#include "ui_usereditor.h"
#include "db/db.h"
#include "db/usertable.h"
#include "entity/product.h"
#include "gui/usermodel.h"
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

    if (db::UserTable::instance()->exists(item.username, item.id)) {
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

    if (!item.id && item.password.isEmpty())  {
        QMessageBox::warning(this, "Peringatan", "Password harus diisi.");
        ui->passwordEdit->setFocus();
        return;
    }

    // ganti password hanya jika diisi
    UserModel::instance()->save(item, item.id == 0 || item.password.length() > 0);

    QDialog::accept();
}
