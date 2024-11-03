#include "datechooserdialog.h"
#include "ui_datechooserdialog.h"

DateChooserDialog::DateChooserDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::DateChooserDialog)
{
    ui->setupUi(this);

    ui->dateEdit->setDate(QDate::currentDate());

    connect(ui->okButton, SIGNAL(clicked()), SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(clicked()), SLOT(reject()));
}

DateChooserDialog::~DateChooserDialog()
{
    delete ui;
}

QDate DateChooserDialog::selectedDate() const
{
    return ui->dateEdit->date();
}
