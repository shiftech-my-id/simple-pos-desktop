#include "common.h"
#include "widgets/application.h"

#include <QMessageBox>
#include <QAbstractButton>
#include <QIcon>
#include <QMap>
#include <QCryptographicHash>

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
