#ifndef CUSTOMERMANAGER_H
#define CUSTOMERMANAGER_H

#include <QWidget>

namespace Ui {
class CustomerManager;
}
class CustomerModel;
class CustomerProxyModel;
class Customer;

class CustomerManager : public QWidget
{
    Q_OBJECT

public:
    explicit CustomerManager(QWidget *parent = nullptr);
    ~CustomerManager();

    Customer currentItem() const;

public slots:
    void add();
    void edit();
    void duplicate();
    void remove();
    void refresh();
    void filter();

private slots:
    void updateButtonState();

private:
    Ui::CustomerManager *ui;
    CustomerModel *model;
    CustomerProxyModel *proxyModel;

    QAction* refreshAction;
    QAction* addAction;
    QAction* duplicateAction;
    QAction* deleteAction;
};

#endif // CUSTOMERMANAGER_H
