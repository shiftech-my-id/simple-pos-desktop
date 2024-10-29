#include "productmodel.h"
#include "db.h"

#include <QApplication>
#include <QColor>

ProductModel::ProductModel(QObject *parent)
    : QAbstractTableModel{parent}
{
}

int ProductModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return items.size();
}

int ProductModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 7;
}

QVariant ProductModel::data(const QModelIndex &index, int role) const
{
    if (!(index.isValid() && index.row() >= 0 && index.row() < items.size()))
        return QVariant();
    Item item = items.at(index.row());
    if (role == Qt::DisplayRole) {
        QLocale locale;
        switch (index.column()) {
        case 0: return item.name;
        case 1: return item.categoryName;
        case 2: return locale.toString(item.stock);
        case 3: return item.uom;
        case 4: return locale.toString(item.cost, 'f', 0);
        case 5: return locale.toString(item.price, 'f', 0);
        case 6: return item.description;
        }
    }
    else if (role == Qt::EditRole) {
        switch (index.column()) {
        case 0: return item.name;
        case 1: return item.categoryName;
        case 2: return item.stock;
        case 3: return item.uom;
        case 4: return item.cost;
        case 5: return item.price;
        case 6: return item.description;
        }
    }
    else if (role == Qt::UserRole) {
        return  item.id;
    }
    else if (role == Qt::BackgroundRole) {
        if (!item.active) {
            return QColor::fromRgb(0xff, 0xaa, 0xaa);
        }
    }
    else if (role == Qt::TextAlignmentRole) {
        switch (index.column()) {
        case 2: return Qt::AlignCenter;
        case 3: return Qt::AlignCenter;
        case 4: return static_cast<int>(Qt::AlignVCenter | Qt::AlignRight);
        case 5: return static_cast<int>(Qt::AlignVCenter | Qt::AlignRight);
        }
    }
    return QVariant();
}

QVariant ProductModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0: return "Kode / Nama Produk";
        case 1: return "Kategori";
        case 2: return "Stok";
        case 3: return "Satuan";
        case 4: return "Harga Beli";
        case 5: return "Harga Jual";
        case 6: return "Deskripsi";
        }
    }
    return QVariant();
}

void ProductModel::refresh()
{
    beginResetModel();
    items.clear();
    QSqlQuery q(db::database());
    q.prepare("select p.*, c.name category_name"
              " from products p"
              " left join product_categories c on c.id = p.category_id"
              " order by p.id asc");
    DB_EXEC(q);
    while (q.next()) {
        Item item;
        item.id = q.value("id").toInt();
        item.name = q.value("name").toString();
        item.description = q.value("description").toString();
        item.categoryName = q.value("category_name").toString();
        item.uom = q.value("uom").toString();
        item.barcode = q.value("barcode").toString();
        item.cost = q.value("cost").toDouble();
        item.price = q.value("price").toDouble();
        item.type = q.value("type").toInt();
        item.stock = q.value("stock").toInt();
        item.active = q.value("active").toBool();
        item.categoryId = q.value("category_id").toInt();
        items.append(item);
    }
    endResetModel();
}
