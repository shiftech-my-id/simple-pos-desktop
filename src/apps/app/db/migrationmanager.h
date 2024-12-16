#ifndef MIGRATIONMANAGER_H
#define MIGRATIONMANAGER_H

#include <QSqlDatabase>
#include <QStringList>

namespace db {

class MigrationManager {
public:
    explicit MigrationManager(QSqlDatabase db);
    void applyMigrations();

private:
    QSqlDatabase m_db;
    QStringList getAppliedMigrations();

    QStringList listMigrationFiles();
    void applyMigration(const QString &name, const QString &sql);
};
} // namespace db

#endif // MIGRATIONMANAGER_H
