#ifndef SALESORDERPROXYMODEL_H
#define SALESORDERPROXYMODEL_H

#include <QSortFilterProxyModel>

class SalesOrderProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit SalesOrderProxyModel(QObject *parent = nullptr);
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

protected:
    bool filterAcceptsRow(int source_row,
                          const QModelIndex &source_parent) const;

public:
    QString searchText;
};

#endif // SALESORDERPROXYMODEL_H
