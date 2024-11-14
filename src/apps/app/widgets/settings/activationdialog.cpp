#include "activationdialog.h"
#include "ui_activationdialog.h"
#include "licensemanager.h"
#include <QMessageBox>

ActivationDialog::ActivationDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ActivationDialog)
{
    ui->setupUi(this);

    ui->snEdit->setText(LicenseManager::instance()->serialNumber());

    connect(ui->okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

ActivationDialog::~ActivationDialog()
{
    delete ui;
}

void ActivationDialog::accept()
{
    QString key = ui->keyEdit->text().trimmed();
    if (key.isEmpty()) {
        QMessageBox::warning(nullptr, "Peringatan", "Masukkan kode aktivasi!");
        ui->keyEdit->setFocus();
        return;
    }
    // 43A658360711A2BD0FAE454960C154798FB1D24B
    if (!LicenseManager::instance()->activate(key)) {
        QMessageBox::warning(nullptr, "Peringatan", "Aktivasi gagal, silahkan periksa kembali kode aktivasi.");
        ui->keyEdit->setFocus();
        return;
    }

    QDialog::accept();
}
