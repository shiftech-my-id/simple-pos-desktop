#ifndef CALCULATORDIALOG_H
#define CALCULATORDIALOG_H

#include <QDialog>

namespace Ui {
class CalculatorDialog;
}

class CalculatorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CalculatorDialog(QWidget *parent = nullptr);
    ~CalculatorDialog();

public slots:
    void onZeroButtonClicked();
    void onOneButtonClicked();
    void onTwoButtonClicked();
    void onThreeButtonClicked();
    void onFourButtonClicked();
    void onFiveButtonClicked();
    void onSixButtonClicked();
    void onSevenButtonClicked();
    void onEightButtonClicked();
    void onNineButtonClicked();
    void onDecimalSeparatorButtonClicked();
    void onEqualButtonClicked();


private:
    void processText();

private:
    Ui::CalculatorDialog *ui;
    double currentResult;
    double currentInput;
};

#endif // CALCULATORDIALOG_H
