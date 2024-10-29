#include "userproxymodel.h"
#include "usermodel.h"

UserProxyModel::UserProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent),
    role(-1),
    showInactive(false)
{
    setSortRole(Qt::EditRole);
}

QVariant UserProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical && role == Qt::DisplayRole) {
        return section + 1;
    }

    return QSortFilterProxyModel::headerData(section, orientation, role);
}

bool UserProxyModel::filterAcceptsRow(int sourceRow,
                                          const QModelIndex &) const
{
    UserModel* model = static_cast<UserModel*>(sourceModel());
    UserModel::Item item = model->items.at(sourceRow);

    bool isAccepted = true;

    if (isAccepted && !searchText.isEmpty()
            && !(item.username.contains(searchText, Qt::CaseInsensitive)
                ||item.fullName.contains(searchText, Qt::CaseInsensitive)
             ))
        isAccepted = false;

    if (isAccepted && (!showInactive && !item.active)) {
        isAccepted = false;
    }

    if (isAccepted && (role != -1 && role != item.role)) {
        isAccepted = false;
    }

    return isAccepted;
}
