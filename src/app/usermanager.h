#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QDialog>

namespace Ui {
class UserManager;
}
class UserModel;
class UserProxyModel;
class User;

class UserManager : public QDialog
{
    Q_OBJECT

public:
    explicit UserManager(QWidget *parent = nullptr);
    ~UserManager();

    User currentItem() const;

public slots:
    void add();
    void edit();
    void remove();
    void refresh();
    void filter();

private slots:
    void updateButtonState();

private:
    Ui::UserManager *ui;
    UserModel *model;
    UserProxyModel *proxyModel;

    QAction* refreshAction;
    QAction* addAction;
    QAction* deleteAction;
};

#endif // USERMANAGER_H
