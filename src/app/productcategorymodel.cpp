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

bool ProductCategoryModel::exists(const QString& name, int id)
{
    QSqlQuery q(db::database());
    // Cek duplikat nama kategori
    if (id == 0) {
        q.prepare("select count(0) from product_categories where name=:name");
    }
    else {
        q.prepare("select count(0) from product_categories where name=:name and id<>:id");
        q.bindValue(":id", id);
    }
    q.bindValue(":name", name);
    if (!DB_EXEC(q)) return false;

    q.next();
    return q.value(0).toInt() > 0;
}

void ProductCategoryModel::save(const Item& item)
{
    QSqlQuery q(db::database());

    if (item.id == 0) {
        q.prepare("insert into product_categories"
                  " ( name) values"
                  " (:name)");
    }
    else {
        q.prepare("update product_categories set"
                  " name=:name"
                  " where id=:id");
        q.bindValue(":id", item.id);
    }
    q.bindValue(":name", item.name);
    DB_EXEC(q);

    int row = -1;
    if (!item.id) {
        const_cast<Item&>(item).id = q.lastInsertId().toInt();
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
        emit dataChanged(index(row, 0), index(row, 0));
    }
}

void ProductCategoryModel::remove(const Item& item)
{

}
