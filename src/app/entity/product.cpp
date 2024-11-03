#include "product.h"
#include <QList>
#include <QMap>


QList<Product::Type> Product::types = {
    Product::Stocked, Product::NonStocked, Product::Serialized, Product::Service
};

QMap<Product::Type, QString> Product::typeNames = {
    {Product::Stocked, "Barang stok"},
    {Product::NonStocked, "Barang tanpa stok"},
    {Product::Serialized, "Barang Berserial"},
    {Product::Service, "Jasa / servis"},
};

Product::Product()
    : fixedMarkup(false),
      fixedMarkup2(false),
      fixedMarkup3(false),
      active(true),
      priceEditable(true),
      categoryId(0),
      supplierId(0),
      type(0),
      stock(0),
      uomQty2(0),
      uomQty3(0),
      cost(0.),
      price(0.),
      markup(0.),
      price2(0.),
      markup2(0.),
      price3(0.),
      markup3(0.)
{
}

QVariantMap Product::toMap() const
{
    QVariantMap map;
    map.insert("id", id ? id : QVariant());
    map.insert("name", name);
    map.insert("description", description);
    map.insert("category_name", categoryName);
    map.insert("uom", uom);
    map.insert("barcode", barcode);
    map.insert("cost", cost);
    map.insert("price", price);
    map.insert("active", active);
    map.insert("stock", stock);
    map.insert("type", type);
    map.insert("uom2", uom2);
    map.insert("uom3", uom3);
    map.insert("uom_qty2", uomQty2);
    map.insert("uom_qty3", uomQty3);
    map.insert("markup", markup);
    map.insert("fixed_markup", fixedMarkup);
    map.insert("price2", price2);
    map.insert("markup2", markup2);
    map.insert("fixed_markup2", fixedMarkup2);
    map.insert("price3", price3);
    map.insert("markup3", markup3);
    map.insert("fixed_markup3", fixedMarkup3);
    map.insert("price_editable", priceEditable);
    map.insert("category_id", categoryId ? categoryId : QVariant());
    map.insert("supplier_id", supplierId ? supplierId : QVariant());
    return map;
}

void Product::fromMap(const QVariantMap &map)
{
    id = map.value("id").toInt();
    name = map.value("name").toString();
    description = map.value("description").toString();
    categoryName = map.value("category_name").toString();
    uom = map.value("uom").toString();
    barcode = map.value("barcode").toString();
    cost = map.value("cost").toDouble();
    price = map.value("price").toDouble();
    active = map.value("active").toBool();
    stock = map.value("stock").toInt();
    type = map.value("type").toInt();
    uom2 = map.value("uom2").toString();
    uom3 = map.value("uom3").toString();
    uomQty2 = map.value("uom_qty2").toInt();
    uomQty3 = map.value("uom_qty3").toInt();
    markup = map.value("markup").toDouble();
    fixedMarkup = map.value("fixed_markup").toBool();
    price2 = map.value("price2").toDouble();
    markup2 = map.value("markup2").toDouble();
    fixedMarkup2 = map.value("fixed_markup2").toBool();
    price3 = map.value("price3").toDouble();
    markup3 = map.value("markup3").toDouble();
    fixedMarkup3 = map.value("fixed_markup3").toBool();
    priceEditable = map.value("price_editable").toBool();
    categoryId = map.value("category_id").toInt();
    supplierId = map.value("supplier_id").toInt();
}
