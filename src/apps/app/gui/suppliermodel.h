#ifndef SUPPLIERMODEL_H
#define SUPPLIERMODEL_H

#include <QAbstractTableModel>
#include "entity/supplier.h"

class SupplierModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    typedef Supplier Item;
    typedef Suppliers Items;

    Items items;

    explicit SupplierModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    void refresh();

};

#endif // SUPPLIERMODEL_H
