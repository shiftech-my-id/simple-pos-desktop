#include "calculatordialog.h"
#include "ui_calculatordialog.h"

#define CALC_RESET_IF_ZERO(t) QString t = ui->currentTextLabel->text();  if (t == "0") {\
t.clear();\
}
#define CALC_SET_CURRENT_TEXT(t) ui->currentTextLabel->setText(t);

CalculatorDialog::CalculatorDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CalculatorDialog)
    , currentResult(0.)
    , currentInput(0.)
{
    ui->setupUi(this);

    connect(ui->zeroButton, SIGNAL(clicked()), SLOT(onZeroButtonClicked()));
    connect(ui->oneButton, SIGNAL(clicked()), SLOT(onOneButtonClicked()));
    connect(ui->twoButton, SIGNAL(clicked()), SLOT(onTwoButtonClicked()));
    connect(ui->threeButton, SIGNAL(clicked()), SLOT(onThreeButtonClicked()));
    connect(ui->fourButton, SIGNAL(clicked()), SLOT(onFourButtonClicked()));
    connect(ui->fiveButton, SIGNAL(clicked()), SLOT(onFiveButtonClicked()));
    connect(ui->sixButton, SIGNAL(clicked()), SLOT(onSixButtonClicked()));
    connect(ui->sevenButton, SIGNAL(clicked()), SLOT(onSevenButtonClicked()));
    connect(ui->eightButton, SIGNAL(clicked()), SLOT(onEightButtonClicked()));
    connect(ui->nineButton, SIGNAL(clicked()), SLOT(onNineButtonClicked()));
    connect(ui->decimalSeparatorButton, SIGNAL(clicked()), SLOT(onDecimalSeparatorButtonClicked()));
    connect(ui->equalButton, SIGNAL(clicked()), SLOT(onEqualButtonClicked()));
}

CalculatorDialog::~CalculatorDialog()
{
    delete ui;
}

void CalculatorDialog::processText()
{

}

void CalculatorDialog::onZeroButtonClicked()
{
    CALC_RESET_IF_ZERO(t);
    t = locale().toString(locale().toDouble(t), 'f', 0);
    t += QString::number(0);
    t = locale().toString(locale().toDouble(t), 'f', 0);
    CALC_SET_CURRENT_TEXT(t);
}

void CalculatorDialog::onOneButtonClicked()
{
    CALC_RESET_IF_ZERO(t);
    t = QString::number(locale().toDouble(t)) + QString::number(1);
    t = locale().toString(locale().toDouble(t), 'f', 0);
    CALC_SET_CURRENT_TEXT(t);
}

void CalculatorDialog::onTwoButtonClicked()
{
    CALC_RESET_IF_ZERO(t);
    t += QString::number(2);
    CALC_SET_CURRENT_TEXT(t);
}

void CalculatorDialog::onThreeButtonClicked()
{
    CALC_RESET_IF_ZERO(t);
    t += QString::number(3);
    CALC_SET_CURRENT_TEXT(t);
}

void CalculatorDialog::onFourButtonClicked()
{
    CALC_RESET_IF_ZERO(t);
    t += QString::number(4);
    CALC_SET_CURRENT_TEXT(t);
}

void CalculatorDialog::onFiveButtonClicked()
{
    CALC_RESET_IF_ZERO(t);
    t += QString::number(5);
    CALC_SET_CURRENT_TEXT(t);
}

void CalculatorDialog::onSixButtonClicked()
{
    CALC_RESET_IF_ZERO(t);
    t += QString::number(6);
    CALC_SET_CURRENT_TEXT(t);
}

void CalculatorDialog::onSevenButtonClicked()
{
    CALC_RESET_IF_ZERO(t);
    t += QString::number(7);
    CALC_SET_CURRENT_TEXT(t);
}

void CalculatorDialog::onEightButtonClicked()
{
    CALC_RESET_IF_ZERO(t);
    t += QString::number(8);
    CALC_SET_CURRENT_TEXT(t);
}

void CalculatorDialog::onNineButtonClicked()
{
    CALC_RESET_IF_ZERO(t);
    t += QString::number(9);
    CALC_SET_CURRENT_TEXT(t);
}

void CalculatorDialog::onDecimalSeparatorButtonClicked()
{

}

void CalculatorDialog::onEqualButtonClicked()
{

}
