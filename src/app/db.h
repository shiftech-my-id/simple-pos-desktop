#ifndef DB_H
#define DB_H

#include <QSqlDatabase>
#include <QSqlQuery>

namespace db {

QSqlDatabase database();
void init();
void resetData();

bool exec(QSqlQuery &q, const QString& file, int line);
bool exec(const QString &str, const QString& file, int line);

}

#define DB_EXEC(Q) db::exec(Q, __FILE__, __LINE__)

#endif // DB_H
