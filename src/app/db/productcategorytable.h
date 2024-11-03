#ifndef DB_PRODUCTCATEGORYTABLE_H
#define DB_PRODUCTCATEGORYTABLE_H

#include "table.h"
#include "libs/core/singleton.h"

namespace db {

class ProductCategoryTable : public Table
{
protected:
    ProductCategoryTable();

public:
    bool exists(const QString& name, int id);
    bool isInUse(int id);

    SINGLETON_INSTANCE(ProductCategoryTable)
    Q_DISABLE_COPY(ProductCategoryTable)
};

} // namespace db

#endif // DB_PRODUCTCATEGORYTABLE_H
