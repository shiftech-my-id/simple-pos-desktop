#ifndef CUSTOMERPROXYMODEL_H
#define CUSTOMERPROXYMODEL_H

#include <QSortFilterProxyModel>

class CustomerProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    CustomerProxyModel(QObject *parent = nullptr);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

public:
    int categoryId;
    int type;
    bool showInactive;
    QString searchText;
};

#endif // CUSTOMERPROXYMODEL_H
