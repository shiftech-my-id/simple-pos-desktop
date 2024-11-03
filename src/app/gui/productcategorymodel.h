#ifndef PRODUCTCATEGORYMODEL_H
#define PRODUCTCATEGORYMODEL_H

#include <QAbstractTableModel>
#include "entity/productcategory.h"
#include "libs/core/singleton.h"

class ProductCategoryModel : public QAbstractTableModel
{
    Q_OBJECT

private:
    explicit ProductCategoryModel(QObject *parent = nullptr);

public:
    typedef ProductCategory Item;
    typedef ProductCategories Items;

    Items items;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    void refresh();

    void save(const Item& item);
    void remove(const Item& item);

    SINGLETON_INSTANCE(ProductCategoryModel)
    Q_DISABLE_COPY(ProductCategoryModel)
};

#endif // PRODUCTCATEGORYMODEL_H
