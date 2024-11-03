#ifndef PRODUCTPROXYMODEL_H
#define PRODUCTPROXYMODEL_H

#include <QSortFilterProxyModel>

class ProductProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    ProductProxyModel(QObject *parent = nullptr);
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

protected:
    bool filterAcceptsRow(int source_row,
                          const QModelIndex &source_parent) const;

public:
    int categoryId;
    int type;
    bool showInactive;
    QString searchText;
};

#endif // PRODUCTPROXYMODEL_H
