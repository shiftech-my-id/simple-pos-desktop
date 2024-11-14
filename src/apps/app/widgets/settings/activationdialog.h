#ifndef ACTIVATIONDIALOG_H
#define ACTIVATIONDIALOG_H

#include <QDialog>

namespace Ui {
class ActivationDialog;
}

class ActivationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ActivationDialog(QWidget *parent = nullptr);
    ~ActivationDialog();

public slots:
    void accept();

private:
    Ui::ActivationDialog *ui;
};

#endif // ACTIVATIONDIALOG_H
