#include "party.h"
#include <QSqlQuery>
#include <QVariant>

Party::Party()
    : active(false),
      type(0),
      balance(0.)
{
}

bool Party::operator==(const Party& o) const
{
    return
        id == o.id &&
        type == o.type &&
        active == o.active &&
        qFuzzyCompare(balance, o.balance) &&
        name == o.name &&
        phone == o.phone &&
        email == o.email &&
        address == o.address;
}

QVariantMap Party::toMap() const
{
    QVariantMap map;
    map.insert("id", id ? id : QVariant());
    map.insert("type", type);
    map.insert("active", active);
    map.insert("balance", balance);
    map.insert("name", name);
    map.insert("phone", phone);
    map.insert("email", email);
    map.insert("address", address);
    return map;
}

void Party::fromMap(const QVariantMap &map)
{
    id = map.value("id").toInt();
    type = map.value("type").value<short>();
    active = map.value("active").toBool();
    balance = map.value("balance").toDouble();
    name = map.value("name").toString();
    phone = map.value("phone").toString();
    email = map.value("email").toString();
    address = map.value("address").toString();
}
