#ifndef NUMBEREDIT_H
#define NUMBEREDIT_H

#include "global.h"
#include <QLineEdit>

class WIDGETS_EXPORT NumberEdit : public QLineEdit
{
    Q_OBJECT

public:
    NumberEdit(QWidget *parent = nullptr);

    void setDecimalPrecision(int prec);
    int decimalPrecision() const;

    void setFormat(char f);
    char format() const;

    double value() const;

public slots:
    void setValue(double value);

protected:
    bool event(QEvent *event);

private:
    int _decimalPrecision;
    char _format;
};

#endif // NUMBEREDIT_H
