#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "companysettingswidget.h"
#include <QSettings>
#include <QLabel>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    connect(ui->saveButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    ui->contentFrame->addWidget("Perusahaan", QIcon(QString::fromUtf8(":/icons/store")),
                                companySettingsWidget = new CompanySettingsWidget(this));
    ui->contentFrame->addWidget("Printer", QIcon(QString::fromUtf8(":/icons/print")), new QLabel("PRINTER", this));
    ui->contentFrame->addWidget("Database", QIcon(QString::fromUtf8(":/icons/database")), new QLabel("DATABASE", this));
    ui->contentFrame->setCurrentIndex(0);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::accept()
{
    companySettingsWidget->save();

    QDialog::accept();
}
