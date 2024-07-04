#ifndef SALESORDERMANAGER_H
#define SALESORDERMANAGER_H

#include <QWidget>
#include "common.h"

namespace Ui {
class SalesOrderManager;
}

class SalesOrderModel;
class SalesOrderProxyModel;

class SalesOrderManager : public QWidget
{
    Q_OBJECT

public:
    explicit SalesOrderManager(QWidget *parent = nullptr);
    ~SalesOrderManager();

signals:
    void productStockChanged();

public slots:
    void refresh();
    void add();
    void edit();
    void remove();
    void filter();
    void updateButtonState();

private:


    StockUpdate currentItem() const;

private:
    Ui::SalesOrderManager *ui;
    SalesOrderModel *model;
    SalesOrderProxyModel *proxyModel;
    QAction* refreshAction;
    QAction* addAction;
    QAction* deleteAction;
};

#endif // SALESORDERMANAGER_H
