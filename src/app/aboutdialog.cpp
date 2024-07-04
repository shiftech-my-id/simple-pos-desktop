#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "global.h"

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    ui->appNameLabel->setText(APP_DISPLAY_NAME);
    ui->appVersionLabel->setText(QString("Versi %1").arg(APP_VERSION_STR));
    ui->qtVersionLabel->setText(QString("Berbasis Qt %1").arg(QT_VERSION_STR));

    connect(ui->okButton, SIGNAL(clicked()), SLOT(accept()));

    adjustSize();
    setFixedSize(size());
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
