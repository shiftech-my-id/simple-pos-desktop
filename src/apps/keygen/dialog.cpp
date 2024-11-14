#include "dialog.h"
#include "ui_dialog.h"
#include "licensemanager.h"
#include <QMessageBox>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    connect(ui->closeButton, &QPushButton::clicked, this, &Dialog::close);
    connect(ui->okButton, &QPushButton::clicked, this, &Dialog::generateActivationKey);
    connect(ui->resetButton, &QPushButton::clicked, this, &Dialog::reset);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::generateActivationKey()
{
    QString sn = ui->snEdit->text().trimmed();
    if (sn.isEmpty()) {
        QMessageBox::warning(nullptr, "Peringatan", "Masukkan SN produk!");
        ui->snEdit->setFocus();
        return;
    }

    QString key = LicenseManager::instance()->generateActivationKey(sn);
    ui->keyEdit->setText(key);
    ui->keyEdit->selectAll();
}

void Dialog::reset()
{
    ui->snEdit->clear();
    ui->keyEdit->clear();
}
