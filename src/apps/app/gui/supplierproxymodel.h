#ifndef SUPPLIERPROXYMODEL_H
#define SUPPLIERPROXYMODEL_H

#include <QSortFilterProxyModel>

class SupplierProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    SupplierProxyModel(QObject *parent = nullptr);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

public:
    int categoryId;
    int type;
    bool showInactive;
    QString searchText;
};

#endif // SUPPLIERPROXYMODEL_H
