#include "productcategoryproxymodel.h"
#include "productcategorymodel.h"

ProductCategoryProxyModel::ProductCategoryProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    setSortRole(Qt::EditRole);
}

QVariant ProductCategoryProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical && role == Qt::DisplayRole) {
        return section + 1;
    }

    return QSortFilterProxyModel::headerData(section, orientation, role);
}

bool ProductCategoryProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &) const
{
    ProductCategoryModel* model = static_cast<ProductCategoryModel*>(sourceModel());
    ProductCategoryModel::Item item = model->items.at(sourceRow);

    bool isAccepted = true;

    if (isAccepted && !searchText.isEmpty() && !(item.name.contains(searchText, Qt::CaseInsensitive)))
        isAccepted = false;

    return isAccepted;
}
