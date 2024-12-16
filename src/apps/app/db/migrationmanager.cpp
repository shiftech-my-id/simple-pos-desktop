#include "migrationmanager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>
#include <QFile>
#include <QDir>
#include <QFileInfo>

using namespace db;

MigrationManager::MigrationManager(QSqlDatabase db) : m_db(db) {}

void MigrationManager::applyMigrations() {
    // Ensure the migrations table exists
    QSqlQuery query(m_db);
    query.exec("CREATE TABLE IF NOT EXISTS migrations ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "name TEXT NOT NULL, "
               "applied_at DATETIME DEFAULT CURRENT_TIMESTAMP)");

    QStringList migrationFiles = listMigrationFiles();
    if (migrationFiles.isEmpty()) {
        qDebug() << "No migration files found in resources.";
        return;
    }

    // Load applied migrations
    QStringList appliedMigrations = getAppliedMigrations();

    // Apply pending migrations
    for (const QString &filePath : migrationFiles) {
        QString fileName = QFileInfo(filePath).baseName();
        if (!appliedMigrations.contains(fileName)) {
            // Read the SQL file
            QFile file(filePath);
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                qWarning() << "Failed to open migration file:" << filePath;
                continue;
            }

            QString sql = file.readAll();
            file.close();

            // Apply the migration
            applyMigration(fileName, sql);
        }
    }
}

QStringList MigrationManager::listMigrationFiles() {
    QStringList files;

    const QString resourceDirPath = ":/resources/migrations";
    // Debug the resource directory
    QDir resourceDir(resourceDirPath);
    if (!resourceDir.exists()) {
        qWarning() << "Resource directory does not exist:" << resourceDirPath;
        return files;
    }

    // List all .sql files
    QStringList fileList = resourceDir.entryList({"*.sql"}, QDir::Files);
    if (fileList.isEmpty()) {
        qWarning() << "No .sql files found in resource directory:" << resourceDirPath;
        return files;
    }

    for (const QString &fileName : fileList) {
        files.append(resourceDirPath + "/" + fileName);
    }

    return files;
}

QStringList MigrationManager::getAppliedMigrations() {
    QStringList appliedMigrations;
    QSqlQuery query(m_db);
    query.exec("SELECT name FROM migrations");

    while (query.next()) {
        appliedMigrations << query.value(0).toString();
    }
    return appliedMigrations;
}

void MigrationManager::applyMigration(const QString &name, const QString &sql) {
    QSqlQuery query(m_db);
    QStringList sqlStatements = sql.split(";", Qt::SkipEmptyParts);

    for (const QString &statement : sqlStatements) {
        // Trim leading and trailing whitespaces
        QString trimmedStatement = statement.trimmed();
        if (trimmedStatement.isEmpty()) {
            continue; // Skip empty statements
        }

        // Execute the SQL statement
        if (!query.exec(trimmedStatement)) {
            qWarning() << "Failed to execute statement:" << query.lastError().text();
            return;
        }
    }

    // Record the migration
    query.prepare("INSERT INTO migrations (name) VALUES (:name)");
    query.bindValue(":name", name);
    if (!query.exec()) {
        qWarning() << "Failed to record migration:" << name << query.lastError();
    }
}
