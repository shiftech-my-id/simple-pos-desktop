#include "supplierproxymodel.h"
#include "suppliermodel.h"

SupplierProxyModel::SupplierProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent),
    categoryId(0),
    type(-1),
    showInactive(false)
{
    setSortRole(Qt::EditRole);
}

QVariant SupplierProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical && role == Qt::DisplayRole) {
        return section + 1;
    }

    return QSortFilterProxyModel::headerData(section, orientation, role);
}

bool SupplierProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &) const
{
    SupplierModel* model = static_cast<SupplierModel*>(sourceModel());
    SupplierModel::Item item = model->items.at(sourceRow);

    bool isAccepted = true;

    if (isAccepted && !searchText.isEmpty() && !(
        item.name.contains(searchText, Qt::CaseInsensitive)
        || item.address.contains(searchText, Qt::CaseInsensitive)
        || item.phone.contains(searchText, Qt::CaseInsensitive)))
        isAccepted = false;

    if (isAccepted && (!showInactive && !item.active)) {
        isAccepted = false;
    }

    return isAccepted;
}
