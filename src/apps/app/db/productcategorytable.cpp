#include "productcategorytable.h"

using namespace db;

ProductCategoryTable::ProductCategoryTable()
    : Table("product_categories", QStringList() << "id")
{
}

bool ProductCategoryTable::exists(const QString &name, int id)
{
    _resetLastState();
    QSqlQuery q(db::connection());
    if (id == 0) {
        q.prepare("select count(0) from product_categories where name=:name");
    }
    else {
        q.prepare("select count(0) from product_categories where name=:name and id<>:id");
        q.bindValue(":id", id);
    }
    q.bindValue(":name", name);
    return _exec(q) && q.next() && q.value(0).toInt() > 0;
}

bool ProductCategoryTable::isInUse(int id)
{
    _resetLastState();
    QSqlQuery q(db::connection());
    q.prepare("select count(0) from products where category_id=?");
    q.bindValue(0, id);
    return _exec(q) && q.next() && q.value(0).toInt() > 0;
}
