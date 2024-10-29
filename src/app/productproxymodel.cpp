#include "productproxymodel.h"
#include "productmodel.h"

ProductProxyModel::ProductProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent),
    categoryId(0),
    type(-1),
    showInactive(false)
{
    setSortRole(Qt::EditRole);
}

QVariant ProductProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical && role == Qt::DisplayRole) {
        return section + 1;
    }

    return QSortFilterProxyModel::headerData(section, orientation, role);
}

bool ProductProxyModel::filterAcceptsRow(int sourceRow,
                                          const QModelIndex &) const
{
    ProductModel* model = static_cast<ProductModel*>(sourceModel());
    ProductModel::Item item = model->items.at(sourceRow);

    bool isAccepted = true;

    if (isAccepted && !searchText.isEmpty()
            && !(item.name.contains(searchText, Qt::CaseInsensitive)
                ||item.uom.contains(searchText, Qt::CaseInsensitive)
                ||item.categoryName.contains(searchText, Qt::CaseInsensitive)
                ||item.description.contains(searchText, Qt::CaseInsensitive)
             ))
        isAccepted = false;

    if (isAccepted && (!showInactive && !item.active)) {
        isAccepted = false;
    }

    if (isAccepted && (type != -1 && type != item.type)) {
        isAccepted = false;
    }

    return isAccepted;
}
