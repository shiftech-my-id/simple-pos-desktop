#ifndef PURCHASEORDERMANAGER_H
#define PURCHASEORDERMANAGER_H

#include <QWidget>
#include "common.h"

namespace Ui {
class PurchaseOrderManager;
}

class PurchaseOrderModel;
class PurchaseOrderProxyModel;

class PurchaseOrderManager : public QWidget
{
    Q_OBJECT

public:
    explicit PurchaseOrderManager(QWidget *parent = nullptr);
    ~PurchaseOrderManager();

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
    Ui::PurchaseOrderManager *ui;
    PurchaseOrderModel *model;
    PurchaseOrderProxyModel *proxyModel;
    QAction* refreshAction;
    QAction* addAction;
    QAction* deleteAction;
};

#endif // PURCHASEORDERMANAGER_H
