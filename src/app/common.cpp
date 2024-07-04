#include "common.h"

#include <QMessageBox>
#include <QAbstractButton>
#include <QIcon>
#include <QMap>

QList<Product::Type> Product::types = {
    Product::NonStocked, Product::Stocked, Product::Service
};

QMap<Product::Type, QString> Product::typeNames = {
    {Product::NonStocked, "Non Stok"},
    {Product::Stocked, "Stok"},
    {Product::Service, "Jasa"},
};

int msgBoxQuestion(QWidget* parent, const QString &title, const QString &text) {
    QMessageBox msgBox(parent);
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    QAbstractButton* yesButton = msgBox.button(QMessageBox::Yes);
    yesButton->setText("&Ya");
    QIcon yesIcon;
    yesIcon.addFile(QString::fromUtf8(":/icons/done"), QSize(), QIcon::Normal, QIcon::Off);
    yesButton->setIcon(yesIcon);

    QAbstractButton* noButton = msgBox.button(QMessageBox::No);
    noButton->setText("&Tidak");
    QIcon noIcon;
    noIcon.addFile(QString::fromUtf8(":/icons/clear"), QSize(), QIcon::Normal, QIcon::Off);
    noButton->setIcon(noIcon);
    return msgBox.exec();
}
