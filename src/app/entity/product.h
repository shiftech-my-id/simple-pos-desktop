#ifndef PRODUCT_H
#define PRODUCT_H

#include "abstractentity.h"
#include <QString>

struct Product : public AbstractEntity
{
    enum Type {
        Stocked = 0,
        NonStocked = 1,
        Serialized = 2,
        Service = 3
    };

    static QList<Product::Type> types;
    static QMap<Product::Type, QString> typeNames;

    Product();

    QVariantMap toMap() const;
    void fromMap(const QVariantMap &d);

    bool fixedMarkup;
    bool fixedMarkup2;
    bool fixedMarkup3;
    bool active;
    bool priceEditable;
    bool __1;
    short __2;
    int categoryId;
    int supplierId;
    int type;
    int stock;
    int uomQty2;
    int uomQty3;
    double cost;
    double price;
    double markup;
    double price2;
    double markup2;
    double price3;
    double markup3;

    QString description;
    QString categoryName;
    QString name;
    QString barcode;
    QString uom;
    QString uom2;
    QString uom3;

    int __3;
};

typedef QList<Product> Products;

#endif // PRODUCT_H
