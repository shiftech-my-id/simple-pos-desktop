#ifndef CUSTOMERMODEL_H
#define CUSTOMERMODEL_H

#include <QAbstractTableModel>
#include "common.h"

class CustomerModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    typedef Customer Item;
    typedef Customers Items;

    Items items;

    explicit CustomerModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    void refresh();

};

#endif // CUSTOMERMODEL_H
