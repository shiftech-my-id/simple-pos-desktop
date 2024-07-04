#ifndef PURCHASEORDERMODEL_H
#define PURCHASEORDERMODEL_H

#include <QAbstractTableModel>
#include "common.h"

class PurchaseOrderModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    typedef StockUpdate Item;
    typedef PurchaseOrders Items;

    Items items;

    explicit PurchaseOrderModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    void refresh(const QString& ops);
};

#endif // PURCHASEORDERMODEL_H
