#ifndef STOCKUPDATEDETAIL_H
#define STOCKUPDATEDETAIL_H

#include <QString>
#include "abstractentity.h"

struct StockUpdateDetail : public AbstractEntity
{
    int updateId;
    int productId;
    int quantity;
    double cost;
    double price;
    double subtotalCost;
    double subtotalPrice;

    QString productName;
    QString productUom;

    StockUpdateDetail();

    virtual QVariantMap toMap() const;
    virtual void fromMap(const QVariantMap &o);
};

typedef QList<StockUpdateDetail> PurchaseOrderDetails;
typedef QList<StockUpdateDetail> SalesOrderDetails;

#endif // STOCKUPDATEDETAIL_H
