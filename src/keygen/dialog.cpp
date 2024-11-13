#include "dialog.h"
#include "ui_dialog.h"
#include <QCryptographicHash>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    connect(ui->closeButton, &QPushButton::clicked, this, &Dialog::close);
    connect(ui->okButton, &QPushButton::clicked, this, &Dialog::generateActivationKey);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::generateActivationKey()
{
    QString sn = ui->snEdit->text().trimmed();
    sn.prepend("SHX-");
    sn.append("-2024");
    QString key = QCryptographicHash::hash(sn.toLocal8Bit(), QCryptographicHash::Sha1).toHex().toUpper();
    ui->keyEdit->setText(key);
    ui->keyEdit->selectAll();
}
