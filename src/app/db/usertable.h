#ifndef DB_USERTABLE_H
#define DB_USERTABLE_H

#include "libs/core/singleton.h"
#include "db/table.h"

namespace db {

class UserTable : public db::Table
{
private:
    UserTable();

public:
    bool exists(const QString& username, int id);

    SINGLETON_INSTANCE(UserTable)
    Q_DISABLE_COPY(UserTable)
};

} // namespace db

#endif // DB_USERTABLE_H
