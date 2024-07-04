#ifndef DB_H
#define DB_H

#include <QSqlDatabase>
#include <QSqlQuery>

#include "common.h"

namespace db {

QSqlDatabase database();
void init();
bool exec(QSqlQuery &q, const QString& file, int line);
bool exec(const QString &str, const QString& file, int line);

}

#define DB_EXEC(Q) db::exec(Q, __FILE__, __LINE__)

#endif // DB_H
