#ifndef SALESORDERMODEL_H
#define SALESORDERMODEL_H

#include <QAbstractTableModel>
#include "entity/stockupdate.h"

class SalesOrderModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    typedef StockUpdate Item;
    typedef SalesOrders Items;

    Items items;

    explicit SalesOrderModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    void refresh(const QString& ops);
};

#endif // SALESORDERMODEL_H
