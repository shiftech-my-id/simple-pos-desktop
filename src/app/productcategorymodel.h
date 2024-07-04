#ifndef PRODUCTCATEGORYMODEL_H
#define PRODUCTCATEGORYMODEL_H

#include <QAbstractTableModel>
#include "common.h"

class ProductCategoryModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    typedef ProductCategory Item;
    typedef ProductCategories Items;

    Items items;

    explicit ProductCategoryModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    void refresh();

private:
};

#endif // PRODUCTCATEGORYMODEL_H
