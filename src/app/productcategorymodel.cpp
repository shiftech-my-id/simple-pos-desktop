#include "productcategorymodel.h"
#include "db.h"

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
    if (role == Qt::EditRole) {
        switch (index.column()) {
        case 0: return item.name;
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

