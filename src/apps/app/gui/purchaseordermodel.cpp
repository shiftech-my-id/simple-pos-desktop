#include "purchaseordermodel.h"
#include "db/db.h"
#include <QLocale>

PurchaseOrderModel::PurchaseOrderModel(QObject *parent)
    : QAbstractTableModel{parent}
{
}

int PurchaseOrderModel::rowCount(const QModelIndex &) const
{
    return items.size();
}

int PurchaseOrderModel::columnCount(const QModelIndex &) const
{
    return 5;
}

QVariant PurchaseOrderModel::data(const QModelIndex &index, int role) const
{
    if (!(index.isValid() && index.row() >= 0 && index.row() < items.size()))
        return QVariant();
    Item item = items.at(index.row());
    if (role == Qt::DisplayRole) {
        QLocale locale;
        switch (index.column()) {
        case 0: return item.purchaseOrderCode();
        case 1: return locale.toString(item.dateTime, "dd-MM-yyyy HH:mm:ss");
        case 2: return item.partyName;
        case 3: return item.partyAddress;
        case 4: return locale.toString(item.totalCost, 'f', 0);
        }
    }
    else if (role == Qt::TextAlignmentRole) {
        switch (index.column()) {
        case 1: return Qt::AlignCenter;
        case 4: return static_cast<int>(Qt::AlignVCenter | Qt::AlignRight);
        }
    }
    return QVariant();
}

QVariant PurchaseOrderModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0: return "No #";
        case 1: return "Tanggal";
        case 2: return "Pemasok";
        case 3: return "Alamat";
        case 4: return "Total";
        }
    }
    return QVariant();
}

void PurchaseOrderModel::refresh(const QString& ops)
{
    QString option = ops.toLower();
    beginResetModel();
    items.clear();

    QString sql = "";
    sql += "select * from stock_updates where type=" + QString::number(StockUpdate::PurchaseOrder) + " and ";
    if (option == "hari ini"
        || option == "kemarin") {
        sql += " date(datetime)=:date";
    }
    else if (option == "7 hari terakhir"
        || option == "30 hari terakhir"
        || option == "bulan ini") {
        sql += " date(datetime) between :date1 and :date2";
    }
    else {
        sql += " 1";
    }
    sql += " order by id desc";

    QSqlQuery q(db::database());
    q.prepare(sql);
    if (option == "hari ini") {
        q.bindValue(":date", QDate::currentDate());
    }
    else if (option == "kemarin") {
        q.bindValue(":date", QDate::currentDate().addDays(-1));
    }
    else if (option == "7 hari terakhir") {
        QDate today = QDate::currentDate();
        q.bindValue(":date1", today.addDays(-7));
        q.bindValue(":date2", today);
    }
    else if (option == "30 hari terakhir") {
        QDate today = QDate::currentDate();
        q.bindValue(":date1", today.addDays(-30));
        q.bindValue(":date2", today);
    }
    else if (option == "bulan ini") {
        QDate today = QDate::currentDate();
        QDate start(today.year(), today.month(), 1);
        QDate end(today.year(), today.month(), today.daysInMonth());
        q.bindValue(":date1", start);
        q.bindValue(":date2", end);
    }

    DB_EXEC(q);
    while (q.next()) {
        Item item;
        item.id = q.value("id").toInt();
        item.dateTime = q.value("datetime").toDateTime();
        item.partyName = q.value("party_name").toString();
        item.partyAddress = q.value("party_address").toString();
        item.totalCost = q.value("total_cost").toDouble();
        items.append(item);
    }
    endResetModel();
}
