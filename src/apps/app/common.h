#ifndef COMMON_H
#define COMMON_H

#include <QDate>

class QWidget;

int msgBoxQuestion(QWidget* parent, const QString &title, const QString &text);

QString encryptPassword(const QString& pass);

#endif // COMMON_H
