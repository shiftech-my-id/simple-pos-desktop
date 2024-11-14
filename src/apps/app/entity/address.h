#ifndef ADDRESS_H
#define ADDRESS_H

#include "abstractentity.h"

class Address : public AbstractEntity
{
public:
    Address();

    QVariantMap toMap() const;
    void fromMap(const QVariantMap &o);

    QString street1;
    QString street2;
    QString city;
    QString province;
    QString country;
    QString postalCode;
    QString notes;
};

#endif // ADDRESS_H
