#include "numberedit.h"
#include <QEvent>
#include <QLocale>
#include <QRegularExpressionValidator>
#include <QRegularExpression>

NumberEdit::NumberEdit(QWidget *parent)
    : QLineEdit(parent)
    , _decimalPrecision(0)
    , _format('f')
{
    setValidator(new QRegularExpressionValidator(
        QRegularExpression(QString("^\\d*|[1-9]\\d{0,2}(\\%1\\d{3})*(%2\\d+)?$")
                           .arg(locale().groupSeparator(), locale().decimalPoint())), this)
    );
}

void NumberEdit::setDecimalPrecision(int prec)
{
    _decimalPrecision = prec;
}

int NumberEdit::decimalPrecision() const
{
    return _decimalPrecision;
}

void NumberEdit::setFormat(char f)
{
    _format = f;
}

char NumberEdit::format() const
{
    return _format;
}

bool NumberEdit::event(QEvent *event)
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

void NumberEdit::setValue(double value)
{
    setText(locale().toString(value, format(), decimalPrecision()));
}

double NumberEdit::value() const
{
    return locale().toDouble(text());
}
