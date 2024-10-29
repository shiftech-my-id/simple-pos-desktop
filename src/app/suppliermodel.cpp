#include "suppliermodel.h"
#include "db.h"

#include <QApplication>
#include <QColor>

SupplierModel::SupplierModel(QObject *parent)
    : QAbstractTableModel{parent}
{
}

int SupplierModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return items.size();
}

int SupplierModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 4;
}

QVariant SupplierModel::data(const QModelIndex &index, int role) const
{
    if (!(index.isValid() && index.row() >= 0 && index.row() < items.size()))
        return QVariant();
    Item item = items.at(index.row());
    if (role == Qt::DisplayRole) {
        QLocale locale;
        switch (index.column()) {
        case 0: return item.name;
        case 1: return item.phone;
        case 2: return locale.toString(item.balance, 'f', 0);
        case 3: return item.address;
        }
    }
    else if (role == Qt::EditRole) {
        switch (index.column()) {
        case 0: return item.name;
        case 1: return item.phone;
        case 2: return item.balance;
        case 3: return item.address;
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
        case 2: return static_cast<int>(Qt::AlignVCenter | Qt::AlignRight);
        }
    }
    return QVariant();
}

QVariant SupplierModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0: return "Nama Supplier";
        case 1: return "Telepon";
        case 2: return "Utang / Piutang";
        case 3: return "Alamat";
        }
    }
    return QVariant();
}

void SupplierModel::refresh()
{
    beginResetModel();
    items.clear();
    QSqlQuery q(db::database());
    q.prepare("select p.*"
              " from parties p"
              " where type=:type"
              " order by p.id asc");
    q.bindValue(":type", Party::Supplier);
    DB_EXEC(q);
    while (q.next()) {
        Item item;
        item.id = q.value("id").toInt();
        item.name = q.value("name").toString();
        item.phone = q.value("phone").toString();
        item.address = q.value("address").toString();
        item.balance = q.value("balance").toDouble();
        item.active = q.value("active").toBool();
        items.append(item);
    }
    endResetModel();
}
