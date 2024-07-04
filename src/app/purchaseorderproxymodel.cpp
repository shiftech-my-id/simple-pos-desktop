#include "purchaseorderproxymodel.h"
#include "purchaseordermodel.h"

PurchaseOrderProxyModel::PurchaseOrderProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{

}

QVariant PurchaseOrderProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical && role == Qt::DisplayRole) {
        return section + 1;
    }

    return QSortFilterProxyModel::headerData(section, orientation, role);
}

bool PurchaseOrderProxyModel::filterAcceptsRow(int sourceRow,
                                          const QModelIndex &) const
{
    PurchaseOrderModel* model = static_cast<PurchaseOrderModel*>(sourceModel());
    PurchaseOrderModel::Item item = model->items.at(sourceRow);

    bool isAccepted = true;

    if (isAccepted && !searchText.isEmpty()
            && !(item.partyName.contains(searchText, Qt::CaseInsensitive)
                ||item.partyAddress.contains(searchText, Qt::CaseInsensitive)))
        isAccepted = false;

    return isAccepted;
}
