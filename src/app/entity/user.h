#ifndef USER_H
#define USER_H

#include "abstractentity.h"

class User : public AbstractEntity {
public:
    enum Role {
        NoRole = 0,
        Administrator = 1,
        StandardUser= 2,
    };

    int id;
    int role;
    bool active;
    QString username;
    QString fullName;
    QString password;

    inline User() : id(0), role(0), active(true) {}

    bool operator==(const User& o) const;
    inline bool operator!=(const User& o) const
    { return !operator==(o); }

    QVariantMap toMap() const;
    void fromMap(const QVariantMap &d);

    static QList<User::Role> roles;
    static QMap<User::Role, QString> roleNames;
};

typedef QList<User> Users;

#endif // USER_H
