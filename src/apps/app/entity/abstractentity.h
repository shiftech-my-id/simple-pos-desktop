#ifndef ABSTRACTENTITY_H
#define ABSTRACTENTITY_H

#include <QVariantMap>

class AbstractEntity
{
protected:
    AbstractEntity();

public:
    int id;

    virtual ~AbstractEntity();

    virtual QVariantMap toMap() const = 0;
    virtual void fromMap(const QVariantMap &o) = 0;
};

#endif // ABSTRACTENTITY_H
