#include "settingtable.h"

using namespace db;

SettingTable::SettingTable()
    : Table("settings", QStringList() << "key")
{
}

bool SettingTable::set(const QString& key, const QVariant& val)
{
    QSqlQuery q(db::connection());
    q.prepare("select count(0) from settings where `key`=?");
    q.bindValue(0, key);
    if (!DB_EXEC(q)) return false;
    if (!q.next()) return false;

    if (q.value(0).toInt() == 0) {
        q.prepare("insert into settings(`key`, `value`)"
                  " values(:key,:value)");
    }
    else {
        q.prepare("update settings set"
                  " `value`=:value where `key`=:key");
    }
    q.bindValue(":key", key);
    q.bindValue(":value", val);
    return DB_EXEC(q);
}

QVariant SettingTable::get(const QString& key, const QVariant& def)
{
    QSqlQuery q(db::connection());
    q.prepare("select `value` from settings where `key`=?");
    q.bindValue(0, key);
    if (!DB_EXEC(q)) return def;
    if (!q.next()) return def;
    return q.value(0);
}
