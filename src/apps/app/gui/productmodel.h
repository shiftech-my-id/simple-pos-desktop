#ifndef PRODUCTMODEL_H
#define PRODUCTMODEL_H

#include <QAbstractTableModel>
#include "entity/product.h"
#include "singleton.h"

class ProductModel : public QAbstractTableModel
{
    Q_OBJECT

signals:
    void totalChanged();

public:
    typedef Product Item;
    typedef Products Items;

    Items items;

    explicit ProductModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    void refresh();

    SINGLETON_INSTANCE(ProductModel)

    inline double totalPrice() const { return _totalPrice; }

private:
    double _totalPrice;

};

#endif // PRODUCTMODEL_H
