#include "usermodel.h"
#include "db/db.h"

#include <QApplication>
#include <QColor>

UserModel::UserModel(QObject *parent)
    : QAbstractTableModel{parent}
{
}

int UserModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return items.size();
}

int UserModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 3;
}

QVariant UserModel::data(const QModelIndex &index, int role) const
{
    if (!(index.isValid() && index.row() >= 0 && index.row() < items.size()))
        return QVariant();
    Item item = items.at(index.row());
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0: return item.username;
        case 1: return item.fullName;
        case 2: return User::roleNames[static_cast<User::Role>(item.role)];
        }
    }
    else if (role == Qt::EditRole) {
        switch (index.column()) {
        case 0: return item.username;
        case 1: return item.fullName;
        case 2: return item.role;
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
        case 0: return static_cast<int>(Qt::AlignLeft | Qt::AlignVCenter);
        }
    }
    return QVariant();
}

QVariant UserModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0: return "ID Pengguna";
        case 1: return "Nama Lengkap";
        case 2: return "Hak Akses";
        }
    }
    return QVariant();
}

void UserModel::refresh()
{
    beginResetModel();
    items.clear();
    QSqlQuery q(db::database());
    q.prepare("select u.*"
              " from users u"
              " order by u.id asc");
    DB_EXEC(q);
    while (q.next()) {
        Item item;
        item.id = q.value("id").toInt();
        item.username = q.value("username").toString();
        item.fullName = q.value("fullname").toString();
        item.role = q.value("role").toInt();
        item.active = q.value("active").toBool();
        items.append(item);
    }
    endResetModel();
}
