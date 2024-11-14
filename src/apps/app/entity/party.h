#ifndef PARTY_H
#define PARTY_H

#include "abstractentity.h"

class QSqlQuery;

class Party : public AbstractEntity
{
public:
    enum Types {
        Unspecified = 0,
        Customer = 1,
        Supplier = 2
    };

    bool active;
    short type;
    double balance;

    QString name;
    QString phone;
    QString email;
    QString address;

    Party();
    bool operator==(const Party& o) const;
    inline bool operator!=(const Party& o) const
    { return !operator==(o); }

    QVariantMap toMap() const;
    void fromMap(const QVariantMap &d);
};
typedef QList<Party> Parties;

#endif // PARTY_H
