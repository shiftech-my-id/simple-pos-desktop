#ifndef DB_PARTYTABLE_H
#define DB_PARTYTABLE_H

#include "table.h"
#include "libs/core/singleton.h"

namespace db {

class PartyTable : public Table
{
private:
    PartyTable();

public:
    QList<QVariantMap> fetchAll(int type);
    SINGLETON_INSTANCE(PartyTable)
    Q_DISABLE_COPY(PartyTable)
};

} // namespace db

#endif // DB_PARTYTABLE_H
