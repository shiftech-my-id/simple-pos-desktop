#ifndef CURRENCYEDIT_H
#define CURRENCYEDIT_H

#include <QLineEdit>

class CurrencyEdit : public QLineEdit
{
    Q_OBJECT

public:
    CurrencyEdit(QWidget *parent = nullptr);

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

#endif // CURRENCYEDIT_H
