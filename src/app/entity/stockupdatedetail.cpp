#include "stockupdatedetail.h"

StockUpdateDetail::StockUpdateDetail() :
    updateId(0),
    productId(0),
    quantity(0),
    cost(0.),
    price(0.)
{
}

QVariantMap StockUpdateDetail::toMap() const
{
    QVariantMap data;
    data["id"] = id == 0 ? QVariant() : id;
    data["update_id"] = updateId;
    data["product_id"] = productId;
    data["quantity"] = quantity;
    data["cost"] = cost;
    data["price"] = price;
    data["product_name"] = productName;
    data["product_uom"] = productUom;
    return data;
}

void StockUpdateDetail::fromMap(const QVariantMap &o)
{
    id = o.value("id").toInt();
    updateId = o.value("update_id").toInt();
    productId = o.value("product_id").toInt();
    quantity = o.value("quantity").toInt();
    cost = o.value("cost").toDouble();
    price = o.value("price").toDouble();
    productName = o.value("product_name").toString();
    productUom = o.value("product_uom").toString();
}
