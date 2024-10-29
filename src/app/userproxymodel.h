#ifndef USERPROXYMODEL_H
#define USERPROXYMODEL_H

#include <QSortFilterProxyModel>

class UserProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    UserProxyModel(QObject *parent = nullptr);
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

protected:
    bool filterAcceptsRow(int source_row,
                          const QModelIndex &source_parent) const;

public:
    int role;
    bool showInactive;
    QString searchText;
};

#endif // USERPROXYMODEL_H
