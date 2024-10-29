#include "currencyedit.h"
#include <QEvent>
#include <QLocale>
#include <QRegularExpressionValidator>
#include <QRegularExpression>

CurrencyEdit::CurrencyEdit(QWidget *parent)
    : QLineEdit(parent)
    , _decimalPrecision(0)
    , _format('f')
{
    setValidator(new QRegularExpressionValidator(
                                 //"^(?:-)?\\d*|[1-9]\\d{0,2}(\\%1\\d{3})*(%2\\d+)?$")
        QRegularExpression(QString("^(?:-)?(?:\\d+|\\d{1,3}(?:%1\\d{3})*)(?:%2\\d+)?$").arg(locale().groupSeparator(), locale().decimalPoint())), this)
    );
}

void CurrencyEdit::setDecimalPrecision(int prec)
{
    _decimalPrecision = prec;
}

int CurrencyEdit::decimalPrecision() const
{
    return _decimalPrecision;
}

void CurrencyEdit::setFormat(char f)
{
    _format = f;
}

char CurrencyEdit::format() const
{
    return _format;
}

bool CurrencyEdit::event(QEvent *event)
{
    if (event->type() == QEvent::FocusOut) {
        QString t = text().trimmed();

        if (t.isEmpty()) {
            t = "0";
        }
        else if (t.endsWith(".") || t.endsWith(",")) {
            t = t.left(t.size() - 1);
        }
        else {
            t = locale().toString(locale().toDouble(t), 'f', 0);
        }

        setText(t);
    }

    return QLineEdit::event(event);
}

void CurrencyEdit::setValue(double value)
{
    setText(locale().toString(value, format(), decimalPrecision()));
}

double CurrencyEdit::value() const
{
    return locale().toDouble(text());
}
