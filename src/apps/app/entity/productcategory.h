#ifndef PRODUCTCATEGORY_H
#define PRODUCTCATEGORY_H

#include <QString>
#include "abstractentity.h"

struct ProductCategory : public AbstractEntity
{
    QString name;

    ProductCategory();
    bool operator==(const ProductCategory& o) const;
    inline bool operator!=(const ProductCategory& o) const
    { return !operator==(o); }

    QVariantMap toMap() const;
    void fromMap(const QVariantMap &d);
};

typedef QList<ProductCategory> ProductCategories;

#endif // PRODUCTCATEGORY_H
