#include "partytable.h"
#include <QStringList>

using namespace db;

PartyTable::PartyTable()
    : Table("parties", QStringList() << "id")
{
}

QList<QVariantMap> PartyTable::fetchAll(int type)
{
    return findAll(QVariantMap({{"type", type}}));
}
