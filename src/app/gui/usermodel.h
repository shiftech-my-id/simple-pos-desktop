#ifndef USERMODEL_H
#define USERMODEL_H

#include <QAbstractTableModel>
#include "entity/user.h"

class UserModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    typedef User Item;
    typedef Users Items;

    Items items;

    explicit UserModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    void refresh();

};

#endif // USERMODEL_H
