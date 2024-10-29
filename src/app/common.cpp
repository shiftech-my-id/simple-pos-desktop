#include "common.h"
#include "application.h"

#include <QMessageBox>
#include <QAbstractButton>
#include <QIcon>
#include <QMap>
#include <QCryptographicHash>

QList<User::Role> User::roles = {
    User::NoRole, User::Administrator, User::StandardUser
};

QMap<User::Role, QString> User::roleNames = {
    {User::NoRole, "Tidak Ada"},
    {User::Administrator, "Administrator"},
    {User::StandardUser, "Pengguna Biasa"},
};


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
    msgBox.setObjectName("msgBox");
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

    QAbstractButton* yesButton = msgBox.button(QMessageBox::Yes);
    yesButton->setText("&Ya");
    yesButton->setIcon(qApp->qtAwesome()->icon("fa-solid fa-check"));

    QAbstractButton* noButton = msgBox.button(QMessageBox::No);
    noButton->setText("&Tidak");
    noButton->setIcon(qApp->qtAwesome()->icon("fa-solid fa-xmark"));

    return msgBox.exec();
}

QString encryptPassword(const QString& pass)
{
    return QCryptographicHash::hash(pass.toLocal8Bit(), QCryptographicHash::Sha1).toHex();
}
