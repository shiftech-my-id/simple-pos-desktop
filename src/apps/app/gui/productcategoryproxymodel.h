#ifndef PRODUCTCATEGORYPROXYMODEL_H
#define PRODUCTCATEGORYPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

class ProductCategoryProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    ProductCategoryProxyModel(QObject *parent = nullptr);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

public:
    QString searchText;
};

#endif // PRODUCTCATEGORYPROXYMODEL_H
