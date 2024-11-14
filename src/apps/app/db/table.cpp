#include "table.h"
#include <QSqlQuery>
#include <QSqlRecord>

namespace db {

Table::Table(const QString &tableName, const QStringList &pks)
    : _tableName(tableName)
    , _primaryKeys(pks)
    , _lastNumRowsAffected(0)
{
    _fetchQuery = QString("select * from %1").arg(_tableName);
}

QVariantMap Table::recordToMap(const QSqlRecord& record)
{
    QVariantMap row;
    recordToMap(record, row);
    return row;
}

void Table::recordToMap(const QSqlRecord& record, QVariantMap& map)
{
    for (int i = 0; i < record.count(); i++) {
        map.insert(record.fieldName(i), record.value(i));
    }
}

QList<QVariantMap> Table::findAll(const QVariantMap &keys)
{
    _resetLastState();
    QList<QVariantMap> rows;
    QStringList where;
    for (auto column: keys.keys()) {
        where << QString(_tableName + ".%1=:%1").arg(column);
    }
    QString sql = _fetchQuery;
    if (!where.isEmpty())
        sql += QString(" where %1").arg(where.join(","));
    QSqlQuery q(db::connection());
    q.prepare(sql);
    for (auto column: keys.keys()) {
        q.bindValue(QString(":%1").arg(column), keys.value(column));
    }
    if (!_exec(q)) return rows;
    while (q.next()) {
        rows << recordToMap(q.record());
    }
    return rows;
}

QVariantMap Table::find(const QVariantMap &keys, bool *found)
{
    QVariantMap row;
    QList<QVariantMap> rows = findAll(keys);
    if (rows.isEmpty()) {
        if (found) *found = false;
        return row;
    }
    row = rows.first();
    if (found) *found = true;
    return row;
}

bool Table::save(const QVariantMap &data)
{
    QVariantMap pks;
    bool isUpdate = false;
    for (const auto &key: data.keys()) {
        if (_primaryKeys.contains(key)) {
            QVariant value = data.value(key);
            if (!value.isNull() && value.isValid()) {
                pks.insert(key, value);
                isUpdate = true;
            }
        }
    }

    return isUpdate ? update(data, pks) : insert(data);
}

bool Table::insert(const QVariantMap &data)
{
    _resetLastState();

    QStringList cols = data.keys();
    QStringList bindCols;
    for (auto column: cols) {
        bindCols << QString(":").append(column);
    }
    QString sql = QString("INSERT INTO %1 (%2) VALUES (%3)").arg(_tableName, cols.join(","), bindCols.join(","));

    QSqlQuery q(db::connection());
    q.prepare(sql);
    for (auto column: cols) {
        q.bindValue(QString(":").append(column), data.value(column));
    }

    return _exec(q);
}

bool Table::update(const QVariantMap &data, const QVariantMap &keys)
{
    _resetLastState();

    QString sql;

    QStringList values;
    for (auto column: data.keys()) {
        values << QString("%1=:%1").arg(column);
    }

    QStringList where;
    for (auto column: keys.keys()) {
        where << QString("%1=:%1").arg(column);
    }

    sql = QString("UPDATE %1 SET %2 WHERE %3").arg(_tableName, values.join(","), where.join(" AND "));

    QSqlQuery q(db::connection());
    q.prepare(sql);
    for (auto column: data.keys()) {
        q.bindValue(QString(":%1").arg(column), data.value(column));
    }

    for (auto column: keys.keys()) {
        q.bindValue(QString(":%1").arg(column), keys.value(column));
    }

    return _exec(q);
}

bool Table::delete_(const QVariantMap& criteria)
{
    _resetLastState();

    QStringList where;
    for (auto column: criteria.keys()) {
        where << QString("%1=:%1").arg(column);
    }

    QSqlQuery q(db::connection());
    q.prepare(QString("delete from %1 where %2").arg(_tableName, where.join(" AND ")));
    for (auto column: criteria.keys()) {
        q.bindValue(QString(":%1").arg(column), criteria.value(column));
    }

    return _exec(q);
}

bool Table::_exec(QSqlQuery &q)
{
    _lastQuery = q.lastQuery();
    if (!DB_EXEC(q)) {
        _lastError = q.lastError();
        return false;
    }
    _lastInsertId = q.lastInsertId();
    _lastNumRowsAffected = q.numRowsAffected();
    return true;
}

void Table::_resetLastState()
{
    _lastInsertId = QVariant();
    _lastError = QSqlError();
    _lastQuery = QString("");
    _lastNumRowsAffected = 0;
}

} // namespace db
