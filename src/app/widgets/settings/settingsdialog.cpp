#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "global.h"
#include <QSettings>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    QSettings settings(APP_SETTINGS_PATH, QSettings::IniFormat);
    settings.beginGroup("CompanyInfo");
    ui->storeNameEdit->setText(settings.value("company_name").toString());
    ui->storeAddressEdit->setPlainText(settings.value("company_address").toString());
    settings.endGroup();
    settings.beginGroup("Print");
    ui->salesInvoicePaperSizeComboBox->setCurrentIndex(settings.value("sales_invoice_papaer_size", 0).toInt());
    settings.endGroup();

    connect(ui->saveButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::accept()
{
    QSettings settings(APP_SETTINGS_PATH, QSettings::IniFormat);
    settings.beginGroup("CompanyInfo");
    settings.setValue("company_name", ui->storeNameEdit->text().trimmed());
    settings.setValue("company_address", ui->storeAddressEdit->toPlainText().trimmed());
    settings.endGroup();
    settings.beginGroup("Print");
    settings.setValue("sales_invoice_papaer_size", ui->salesInvoicePaperSizeComboBox->currentIndex());
    settings.endGroup();
    QDialog::accept();
}
