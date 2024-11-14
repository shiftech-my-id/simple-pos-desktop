#include "settingtable.h"

using namespace db;

SettingTable::SettingTable()
    : Table("settings", QStringList() << "id")
{
}

bool SettingTable::set(const QString& id, const QVariant& val)
{
    QSqlQuery q(db::connection());
    q.prepare("select count(0) from settings where id=?");
    q.bindValue(0, id);
    if (!DB_EXEC(q)) return false;
    if (!q.next()) return false;

    if (q.value(0).toInt() == 0) {
        q.prepare("insert into settings(id, data)"
                  " values(:id,:data)");
    }
    else {
        q.prepare("update settings set"
                  " data=:data where id=:id");
    }
    q.bindValue(":id", id);
    q.bindValue(":data", val);
    return DB_EXEC(q);
}

QVariant SettingTable::get(const QString& id, const QVariant& def)
{
    QSqlQuery q(db::connection());
    q.prepare("select data from settings where id=?");
    q.bindValue(0, id);
    if (!DB_EXEC(q)) return def;
    if (!q.next()) return def;
    return q.value(0);
}
