#include "reportdatedialog.h"
#include "ui_reportdatedialog.h"
#include <QDate>

ReportDateDialog::ReportDateDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::ReportDateDialog)
{
    ui->setupUi(this);
    ui->yearSpinBox->setValue(QDate::currentDate().year());
    ui->monthComboBox->addItem("Januari", 1);
    ui->monthComboBox->addItem("Februari", 2);
    ui->monthComboBox->addItem("Maret", 3);
    ui->monthComboBox->addItem("April", 4);
    ui->monthComboBox->addItem("Mei", 5);
    ui->monthComboBox->addItem("Juni", 6);
    ui->monthComboBox->addItem("Juli", 7);
    ui->monthComboBox->addItem("Agustus", 8);
    ui->monthComboBox->addItem("September", 9);
    ui->monthComboBox->addItem("Oktober", 10);
    ui->monthComboBox->addItem("November", 11);
    ui->monthComboBox->addItem("Desember", 12);

    ui->monthComboBox->setCurrentIndex(QDate::currentDate().month() - 1);

    connect(ui->okButton, SIGNAL(clicked()), SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(clicked()), SLOT(reject()));
}

ReportDateDialog::~ReportDateDialog()
{
    delete ui;
}

int ReportDateDialog::year() const
{
    return ui->yearSpinBox->value();
}

int ReportDateDialog::month() const
{
    return ui->monthComboBox->currentData().toInt();
}
