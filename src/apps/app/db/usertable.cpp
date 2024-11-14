#include "usertable.h"
#include <QMessageBox>

namespace db {

UserTable::UserTable()
    : Table("users", QStringList() << "id")
{
}

bool UserTable::exists(const QString& username, int id)
{
    QSqlQuery q(db::database());
    // Cek duplikat nama pengguna
    if (id == 0) {
        q.prepare("select count(0) from users where username=:username");
    }
    else {
        q.prepare("select count(0) from users where username=:username and id<>:id");
        q.bindValue(":id", id);
    }
    q.bindValue(":username", username);
    if (!DB_EXEC(q)) return false;
    q.next();
    return q.value(0).toInt();
}

} // namespace db
