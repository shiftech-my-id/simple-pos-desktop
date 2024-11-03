#include "productcategory.h"

ProductCategory::ProductCategory()
{
}

bool ProductCategory::operator==(const ProductCategory& o) const
{
    return o.id == id && o.name == name;
}

QVariantMap ProductCategory::toMap() const
{
    QVariantMap map;
    map.insert("id", id ? id : QVariant());
    map.insert("name", name);
    return map;
}

void ProductCategory::fromMap(const QVariantMap &d)
{
    id = d.value("id").toInt();
    name = d.value("name").toString();
}
