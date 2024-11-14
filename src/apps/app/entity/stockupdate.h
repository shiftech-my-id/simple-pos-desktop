#ifndef STOCKUPDATE_H
#define STOCKUPDATE_H

#include <QDateTime>
#include "abstractentity.h"

struct StockUpdate : public AbstractEntity
{
    enum Type {
        InitialStockAdjustment = 0,
        ManualStockAdjustment = 1,
        SalesOrder = 10,
        SalesOrderReturn = 11,
        PurchaseOrder = 20,
        PurchaseOrderReturn = 21,
    };

    int code;
    int type;
    double total;
    double discount;
    double totalPayment;
    double totalPaid;
    double exchange;
    double totalCost;
    double totalProfit;
    int __1;
    QDateTime dateTime;
    QString partyName;
    QString partyAddress;

    StockUpdate();

    virtual QVariantMap toMap() const;
    virtual void fromMap(const QVariantMap &o);

    static QString purchaseOrderCode(int pCode){
        return QString("PB-%1").arg(QString::number(pCode));
    }

    QString purchaseOrderCode() {
        return purchaseOrderCode(code);
    }

    static QString salesOrderCode(int pCode){
        return QString("PJ-%1").arg(QString::number(pCode));
    }

    QString salesOrderCode() {
        return salesOrderCode(code);
    }
};

typedef QList<StockUpdate> SalesOrders;
typedef QList<StockUpdate> PurchaseOrders;

#endif // STOCKUPDATE_H
