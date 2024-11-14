#include "address.h"

Address::Address()
{
}

QVariantMap Address::toMap() const
{
    QVariantMap data;
    data["street1"] = street1;
    data["street2"] = street2;
    data["city"] = city;
    data["province"] = province;
    data["country"] = country;
    data["postal_code"] = postalCode;
    data["notes"] = notes;
    return data;
}

void Address::fromMap(const QVariantMap &o)
{
    street1 = o.value("street1").toString();
    street2 = o.value("street2").toString();
    city = o.value("city").toString();
    province = o.value("province").toString();
    country = o.value("country").toString();
    postalCode = o.value("postal_code").toString();
    notes = o.value("notes").toString();
}
