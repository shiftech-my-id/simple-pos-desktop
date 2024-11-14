#include "user.h"

QList<User::Role> User::roles = {
    User::NoRole, User::Administrator, User::StandardUser
};

QMap<User::Role, QString> User::roleNames = {
    {User::NoRole, "Tidak Ada"},
    {User::Administrator, "Administrator"},
    {User::StandardUser, "Pengguna Biasa"},
    };

bool User::operator==(const User& o) const
{
    return o.id == id && o.username == username && o.role == role && o.active == active && o.fullName == fullName /* && o.password == password */;
}


QVariantMap User::toMap() const
{
    QVariantMap map;
    map.insert("id", id ? id : QVariant());
    map.insert("role", role);
    map.insert("active", active);
    map.insert("username", username);
    map.insert("fullname", fullName);
    map.insert("password", password);
    return map;
}

void User::fromMap(const QVariantMap &map)
{
    id = map.value("id").toInt();
    role = map.value("type").value<short>();
    active = map.value("active").toBool();
    username = map.value("phone").toString();
    fullName = map.value("name").toString();
    password = map.value("password").toString();
}
