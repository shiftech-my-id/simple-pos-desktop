#include "productcategorymodel.h"
#include "db/db.h"
#include "db/productcategorytable.h"

ProductCategoryModel::ProductCategoryModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

QVariant ProductCategoryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0: return "Nama Kategori";
        }
    }

    return QVariant();
}

int ProductCategoryModel::rowCount(const QModelIndex &) const
{
    return items.size();
}

int ProductCategoryModel::columnCount(const QModelIndex &) const
{
    return 1;
}

QVariant ProductCategoryModel::data(const QModelIndex &index, int role) const
{
    if (!(index.isValid() && index.row() >= 0 && index.row() < items.size()))
        return QVariant();
    Item item = items.at(index.row());
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0: return item.name;
        }
    }
    else if (role == Qt::EditRole) {
        switch (index.column()) {
        case 0: return item.name;
        }
    }
    else if (role == Qt::UserRole) {
        switch (index.column()) {
        case 0: return item.id;
        }
    }

    return QVariant();
}

void ProductCategoryModel::refresh()
{
    beginResetModel();
    items.clear();
    QSqlQuery q(db::database());
    q.prepare("select * from product_categories order by name asc");
    DB_EXEC(q);
    while (q.next()) {
        Item item;
        item.id = q.value("id").toInt();
        item.name = q.value("name").toString();
        items.append(item);
    }
    endResetModel();
}

void ProductCategoryModel::save(const Item& item)
{
    db::ProductCategoryTable::instance()->save(item.toMap());

    int row = -1;
    if (!item.id) {
        const_cast<Item&>(item).id = db::ProductCategoryTable::instance()->lastInsertId().toInt();
        row = items.size();
        beginInsertRows(QModelIndex(), row, row);
        items.append(item);
        endInsertRows();
    }
    else {
        for (int i = 0; i < items.size(); i++) {
            if (items.at(i).id == item.id) {
                row = i;
                break;
            }
        }
        items[row] = item;
        emit dataChanged(index(row, 0), index(row, 0));
    }
}

void ProductCategoryModel::remove(const Item& item)
{
    int row = items.indexOf(item);

    db::ProductCategoryTable::instance()->deleteById(item.id);

    beginRemoveRows(QModelIndex(), row, row);
    items.removeAt(row);
    endRemoveRows();
}
