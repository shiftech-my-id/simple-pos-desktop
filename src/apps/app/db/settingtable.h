#ifndef DB_SETTINGTABLE_H
#define DB_SETTINGTABLE_H

#include "table.h"
#include "singleton.h"

namespace db {

class SettingTable : public Table
{
public:
    static bool set(const QString& id, const QVariant& val);
    static QVariant get(const QString& id, const QVariant& val = QVariant());

protected:
    SettingTable();
    SINGLETON_INSTANCE(SettingTable)
    Q_DISABLE_COPY(SettingTable)
};

} // namespace db

#endif // DB_SETTINGTABLE_H
