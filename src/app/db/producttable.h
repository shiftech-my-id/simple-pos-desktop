#ifndef DB_PRODUCTTABLE_H
#define DB_PRODUCTTABLE_H

#include "table.h"
#include "libs/core/singleton.h"
#include "common/product.h"

namespace db {

class ProductTable : public Table
{
protected:
    ProductTable();

public:
    bool save(const QVariantMap &data) override;
    bool exists(const QString& col, const QString &value, int id);
    inline bool nameExists(const QString& name, int id)
    { return exists("name", name, id); }
    inline bool barcodeExists(const QString& code, int id)
    { return exists("barcode", code, id); }
    void change(const QString& col, const QVariant& oldVal, const QVariant& newVal);
    void changeCategory(const QVariant &oldCatId, const QVariant &newCatId);
    void changeSupplier(const QVariant &oldId, const QVariant &newId);
    QStringList fetchAllUom();

    SINGLETON_INSTANCE(ProductTable)
    Q_DISABLE_COPY(ProductTable)
};

} // namespace db

#endif // DB_PRODUCTTABLE_H
