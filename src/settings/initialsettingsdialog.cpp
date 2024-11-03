#include "initialsettingsdialog.h"
#include "ui_initialsettingsdialog.h"
#include "../app/global.h"

#include <QSettings>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlError>
#include <QPrinter>
#include <QPrinterInfo>
#include <QAbstractItemView>

InitialSettingsDialog::InitialSettingsDialog(QWidget *parent)
    : QDialog(parent, Qt::WindowCloseButtonHint)
    , ui(new Ui::InitialSettingsDialog)
{
    ui->setupUi(this);

    ui->driverComboBox->setEditable(true);
    ui->driverComboBox->lineEdit()->setReadOnly(true);
    ui->driverComboBox->view()->setAlternatingRowColors(true);

    for (const QString &driver : QSqlDatabase::drivers()) {
        ui->driverComboBox->addItem(driver);
    }
    ui->driverComboBox->setCurrentIndex(-1);

    ui->portSpinBox->setMinimum(0);
    ui->portSpinBox->setMaximum(USHRT_MAX);

    connect(ui->okButton, SIGNAL(clicked()), SLOT(saveAndClose()));
    connect(ui->cancelButton, SIGNAL(clicked()), SLOT(close()));
    connect(ui->testButton, SIGNAL(clicked()), SLOT(testDatabaseConnection()));
    connect(ui->driverComboBox, SIGNAL(currentIndexChanged(int)), SLOT(onDriverChanged()));

    QSettings settings(APP_SETTINGS_PATH, QSettings::IniFormat);
    ui->driverComboBox->setCurrentIndex(ui->driverComboBox->findText(settings.value(SK_DATABASE_DRIVER).toString()));
    ui->hostEdit->setText(settings.value(SK_DATABASE_HOSTNAME).toString());
    ui->portSpinBox->setValue(settings.value(SK_DATABASE_PORT).toInt());
    ui->usernameEdit->setText(settings.value(SK_DATABASE_USERNAME).toString());
    ui->passwordEdit->setText(settings.value(SK_DATABASE_PASSWORD).toString());
    ui->databaseName->setText(settings.value(SK_DATABASE_DATABASENAME).toString());
}

InitialSettingsDialog::~InitialSettingsDialog()
{
    delete ui;
}

void InitialSettingsDialog::saveAndClose()
{
    QSettings settings(APP_SETTINGS_PATH, QSettings::IniFormat);
    settings.setValue(SK_DATABASE_DRIVER, ui->driverComboBox->currentText());
    settings.setValue(SK_DATABASE_HOSTNAME, ui->hostEdit->text().trimmed());
    settings.setValue(SK_DATABASE_PORT, ui->portSpinBox->value());
    settings.setValue(SK_DATABASE_USERNAME, ui->usernameEdit->text().trimmed());
    settings.setValue(SK_DATABASE_PASSWORD, ui->passwordEdit->text());
    settings.setValue(SK_DATABASE_DATABASENAME, ui->databaseName->text().trimmed());
    close();
}

void InitialSettingsDialog::testDatabaseConnection()
{
    QSqlError error;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase(ui->driverComboBox->currentText());
        db.setHostName(ui->hostEdit->text().trimmed());
        db.setPort(ui->portSpinBox->value());
        db.setDatabaseName(ui->databaseName->text().trimmed());
        db.setUserName(ui->usernameEdit->text().trimmed());
        db.setPassword(ui->passwordEdit->text().trimmed());
        db.open();

        error = db.lastError();

        db.close();
    }

    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);

    if (error.isValid()) {
        QMessageBox msgbox(this);
        msgbox.setWindowTitle("Pengujian Koneksi");
        msgbox.setText("Gagal terhubung ke basis data.");
        msgbox.setIcon(QMessageBox::Critical);
        msgbox.setDetailedText(error.text());
        msgbox.exec();
        return;
    }

    QMessageBox::information(this, "Pengujian Koneksi", "Berhasil terhubung ke basis data.");
}

void InitialSettingsDialog::onDriverChanged()
{
    bool isSQLite = ui->driverComboBox->currentText() == "QSQLITE";
    ui->hostEdit->setDisabled(isSQLite);
    ui->portSpinBox->setDisabled(isSQLite);
    ui->usernameEdit->setDisabled(isSQLite);
    ui->passwordEdit->setDisabled(isSQLite);

    if (isSQLite) {
        ui->hostEdit->clear();
        ui->portSpinBox->clear();
        ui->usernameEdit->clear();
        ui->passwordEdit->clear();
    }
}
