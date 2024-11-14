#ifndef DB_TABLE_H
#define DB_TABLE_H

#include "db.h"
#include <QSqlError>

namespace db {

class Table
{
public:
    Table(const QString &tableName, const QStringList &pks);

    virtual bool save(const QVariantMap &data);
    bool insert(const QVariantMap &data);
    bool update(const QVariantMap &data, const QVariantMap &keys);
    bool delete_(const QVariantMap &keys);

    QList<QVariantMap> findAll(const QVariantMap &keys = QVariantMap());
    QVariantMap find(const QVariantMap &keys, bool *found = nullptr);
    QVariantMap findById(const QVariant id, bool *found = nullptr)
    { return find(QVariantMap({{"id", id}}), found); }
    virtual inline QList<QVariantMap> fetchAll() { return findAll(QVariantMap()); }

    inline bool updateById(const QVariantMap &data, const QVariant &key)
    { return update(data, QVariantMap({{"id", key}})); }
    inline bool deleteById(const QVariant &id)
    { return delete_(QVariantMap({{"id", id}})); }

    inline QSqlError lastError() const { return _lastError; }
    inline QString lastQuery() const { return _lastQuery; }
    inline QVariant lastInsertId() const { return _lastInsertId; }
    inline int lastNumRowsAffected() const { return _lastNumRowsAffected; }

    static void recordToMap(const QSqlRecord& record, QVariantMap& map);
    static QVariantMap recordToMap(const QSqlRecord& record);

protected:
    void _resetLastState();
    bool _exec(QSqlQuery &query);

    QString _tableName;
    QStringList _primaryKeys;
    QSqlError _lastError;
    QString _lastQuery;
    QVariant _lastInsertId;
    QString _fetchQuery;
    int _lastNumRowsAffected;
};

} // namespace db

#endif // DB_TABLE_H
