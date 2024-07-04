#include "salesordereditormodel.h"
#include "db.h"
#include <QLocale>

SalesOrderEditorModel::SalesOrderEditorModel(QObject *parent)
    : QAbstractTableModel{parent}
    , readOnly(false)
{

}

Qt::ItemFlags SalesOrderEditorModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags f;

    f = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    if (index.column() == 1 && !readOnly)
        f |= Qt::ItemIsEditable;

    return f;
}

int SalesOrderEditorModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return items.size();
}

int SalesOrderEditorModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 5;
}

bool SalesOrderEditorModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.column() == 1 && role == Qt::EditRole) {
        int quantity = value.toInt();
        if (quantity < 1 || quantity > 999999)
            return false;
        updateQuantity(index.row(), quantity);
        return true;
    }
    return false;
}

QVariant SalesOrderEditorModel::data(const QModelIndex &index, int role) const
{
    if (!(index.isValid() && index.row() >= 0 && index.row() < items.size()))
        return QVariant();
    Item item = items.at(index.row());
    if (role == Qt::DisplayRole) {
        QLocale locale;
        switch (index.column()) {
        case 0: return item.productName;
        case 1: return item.quantity;
        case 2: return item.productUom;
        case 3: return locale.toString(item.price, 'f', 0);
        case 4: return locale.toString(item.subtotalPrice, 'f', 0);
        }
    }
    else if (role == Qt::EditRole && index.column() == 1) {
        return item.quantity;
    }
    else if (role == Qt::TextAlignmentRole) {
        switch (index.column()) {
        case 1: return static_cast<int>(Qt::AlignVCenter | Qt::AlignRight);
        case 3: return static_cast<int>(Qt::AlignVCenter | Qt::AlignRight);
        case 4: return static_cast<int>(Qt::AlignVCenter | Qt::AlignRight);
        }
    }
    return QVariant();
}

QVariant SalesOrderEditorModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section) {
            case 0: return "Nama Produk";
            case 1: return "Kwantitas";
            case 2: return "Satuan";
            case 3: return "Harga";
            case 4: return "Jumlah Harga";
            }
        }
        else {
            return section + 1;
        }
    }
    return QVariant();
}

void SalesOrderEditorModel::updateQuantity(int row, int quantity)
{
    Item *item = &items[row];
    item->quantity = quantity;
    item->subtotalCost = item->cost * item->quantity;
    item->subtotalPrice = item->price * item->quantity;
    emit dataChanged(index(row, 1), index(row, 1));
}

void SalesOrderEditorModel::addItem(const StockUpdateDetail &item)
{
    int existingRow = itemIndexByNames.value(item.productName, -1);
    if (existingRow > -1) {
        updateQuantity(existingRow, items.at(existingRow).quantity + item.quantity);
        return;
    }

    int row = items.size();
    beginInsertRows(QModelIndex(), row, row);
    items.append(item);
    itemIndexByNames.insert(item.productName, row);
    endInsertRows();
}

void SalesOrderEditorModel::removeRow(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    itemIndexByNames.remove(items.at(row).productName);
    items.removeAt(row);
    endRemoveRows();
}

void SalesOrderEditorModel::refresh(int orderId)
{
    beginResetModel();
    QSqlQuery q(db::database());
    q.prepare("select * from stock_update_details where update_id=:id");
    q.bindValue(":id", orderId);
    DB_EXEC(q);
    while (q.next()) {
        Item item;
        item.id = q.value("id").toInt();
        item.productName = q.value("product_name").toString();
        item.productUom = q.value("product_uom").toString();
        item.quantity = q.value("quantity").toInt();
        item.price = q.value("price").toDouble();
        item.subtotalPrice = q.value("subtotal_price").toDouble();
        items.append(item);
    }
    endResetModel();
}
