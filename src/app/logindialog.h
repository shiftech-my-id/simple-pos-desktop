#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    void login(int id);
    ~LoginDialog();

public slots:
    void accept();

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
