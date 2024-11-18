#include "producttable.h"

using namespace db;

ProductTable::ProductTable()
    : Table("products", QStringList() << "id")
{
    _fetchQuery = "SELECT p.*, pc.name category_name, s.name supplier_name FROM products p"
                  " LEFT JOIN product_categories pc ON pc.id = p.category_id"
                  " LEFT JOIN parties s ON s.id = p.supplier_id";
}

bool ProductTable::save(const QVariantMap &pData)
{
    QVariantMap data = pData;
    data.remove("category_name");
    return Table::save(data);
}

bool ProductTable::exists(const QString& col, const QString &value, int id)
{
    QSqlQuery q(db::connection());
    if (id == 0) {
        q.prepare(QString("select count(0) from products where %1=:%1").arg(col));
    }
    else {
        q.prepare(QString("select count(0) from products where %1=:%1 and id<>:id").arg(col));
        q.bindValue(":id", id);
    }
    q.bindValue(QString(":%1").arg(col), value);
    if (!DB_EXEC(q))
        return false;

    q.next();
    if (q.value(0).toInt() == 0) {
        return false;
    }

    return true;
}

QStringList ProductTable::fetchAllUom()
{
    _resetLastState();
    QSqlQuery q(db::connection());
    q.prepare("select uom from"
              " (select uom from products group by uom union all"
              " select uom2 from products group by uom2 union all"
              " select uom3 from products group by uom3) uom"
              " group by uom order by uom asc;");
    _exec(q);
    QStringList uoms;
    while (q.next())
        uoms << q.value(0).toString();
    return uoms;
}

void ProductTable::change(const QString& col, const QVariant& oldVal, const QVariant& newVal)
{
    _resetLastState();
    QSqlQuery q(db::connection());
    q.prepare(QString("update products set %1=:new_val where %1=:old_val").arg(col));
    q.bindValue(":new_val", newVal);
    q.bindValue(":old_val", oldVal);
    _exec(q);
}

void ProductTable::changeCategory(const QVariant &oldCatId, const QVariant &newCatId)
{
    change("category_id", oldCatId, newCatId);
}

void ProductTable::changeSupplier(const QVariant &oldId, const QVariant &newId)
{
    change("supplier_id", oldId, newId);
}
