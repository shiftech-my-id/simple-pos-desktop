#ifndef DB_H
#define DB_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

namespace db {

QSqlDatabase connection();
QSqlDatabase database();
void init();
void reset();
bool exec(QSqlQuery &q, const char* file, int line, const char *fn);
bool exec(const QString& sql, const char* file, int line, const char *fn);

}

#define DB_EXEC(Q) db::exec(Q, __FILE__, __LINE__, __PRETTY_FUNCTION__)

#endif // DB_H
