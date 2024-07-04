#ifndef PURCHASEORDERPROXYMODEL_H
#define PURCHASEORDERPROXYMODEL_H

#include <QSortFilterProxyModel>

class PurchaseOrderProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit PurchaseOrderProxyModel(QObject *parent = nullptr);
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

protected:
    bool filterAcceptsRow(int source_row,
                          const QModelIndex &source_parent) const;

public:
    QString searchText;
};

#endif // PURCHASEORDERPROXYMODEL_H
