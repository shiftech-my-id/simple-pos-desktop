#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "companysettingswidget.h"
#include "widgets/application.h"

#include <QSettings>
#include <QLabel>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    connect(ui->saveButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    ui->contentFrame->addWidget("Perusahaan", qApp->qtAwesome()->icon("fa-solid fa-building"),
                                companySettingsWidget = new CompanySettingsWidget(this));
    ui->contentFrame->addWidget("Printer", qApp->qtAwesome()->icon("fa-solid fa-print"), new QLabel("PRINTER", this));
    ui->contentFrame->addWidget("Database", qApp->qtAwesome()->icon("fa-solid fa-database"), new QLabel("DATABASE", this));
    ui->contentFrame->setCurrentIndex(0);

    ui->saveButton->setIcon(qApp->qtAwesome()->icon("fa-solid fa-check"));
    ui->cancelButton->setIcon(qApp->qtAwesome()->icon("fa-solid fa-xmark"));
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
