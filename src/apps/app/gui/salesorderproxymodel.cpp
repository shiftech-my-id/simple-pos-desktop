#include "salesorderproxymodel.h"
#include "salesordermodel.h"

SalesOrderProxyModel::SalesOrderProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{

}

QVariant SalesOrderProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical && role == Qt::DisplayRole) {
        return section + 1;
    }

    return QSortFilterProxyModel::headerData(section, orientation, role);
}

bool SalesOrderProxyModel::filterAcceptsRow(int sourceRow,
                                          const QModelIndex &) const
{
    SalesOrderModel* model = static_cast<SalesOrderModel*>(sourceModel());
    SalesOrderModel::Item item = model->items.at(sourceRow);

    bool isAccepted = true;

    if (isAccepted && !searchText.isEmpty()
            && !(item.partyName.contains(searchText, Qt::CaseInsensitive)
                ||item.partyAddress.contains(searchText, Qt::CaseInsensitive)))
        isAccepted = false;

    return isAccepted;
}
