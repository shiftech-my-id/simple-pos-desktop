#ifndef COMMON_H
#define COMMON_H

#include <QDate>

class QWidget;

int msgBoxQuestion(QWidget* parent, const QString &title, const QString &text);

QString encryptPassword(const QString& pass);

struct User {
    enum Role {
        NoRole = 0,
        Administrator = 1,
        StandardUser= 2,
    };

    int id;
    int role;
    bool active;
    QString username;
    QString fullName;
    QString password;

    User() : id(0), role(0), active(true) {}

    static QList<User::Role> roles;
    static QMap<User::Role, QString> roleNames;
};

struct ProductCategory
{
    int id;
    QString name;

    ProductCategory() : id(0) {}
};

struct Product {
    enum Type {
        NonStocked = 0,
        Stocked = 1,
        Service = 2,
    };

    int id;
    int categoryId;
    int type;
    int stock;
    double cost;
    double price;
    bool active;
    QString categoryName;
    QString name;
    QString description;
    QString barcode;
    QString uom;

    Product() : id(0), categoryId(0), type(0), stock(0), cost(0.), price(0.), active(true) {}

    static QList<Product::Type> types;
    static QMap<Product::Type, QString> typeNames;
};

struct Party {
    enum Type {
        Customer = 0,
        Supplier = 1,
    };

    int id;
    int type;
    bool active;
    double balance;
    QString name;
    QString address;
    QString phone;

    Party() : id(0), type(0), active(false), balance(0.) {}
};

struct Customer : public Party {
    Customer() {
        type = Party::Customer;
    }
};

struct Supplier: public Party {
    Supplier() {
        type = Party::Supplier;
    }
};

struct StockUpdate
{
    enum Type {
        InitialStockAdjustment = 0,
        ManualStockAdjustment = 1,
        SalesOrder = 10,
        PurchaseOrder = 20,
    };

    int id;
    int type;
    QDateTime dateTime;
    double total;
    double discount;
    double totalPayment;
    double totalPaid;
    double exchange;
    double totalCost;
    double totalProfit;
    QString partyName;
    QString partyAddress;
    StockUpdate() : id(0), total(0.), discount(0.),
        totalPayment(0.), totalPaid(0.), exchange(0.),
        totalCost(0.), totalProfit(0.) {}

    static QString purchaseOrderCode(int id){
        return QString("PB-%1").arg(QString::number(id));
    }

    QString purchaseOrderCode() {
        return purchaseOrderCode(id);
    }

    static QString salesOrderCode(int id){
        return QString("PJ-%1").arg(QString::number(id));
    }

    QString salesOrderCode() {
        return salesOrderCode(id);
    }
};

struct StockUpdateDetail
{
    int id;
    int productId;
    int quantity;
    double cost;
    double price;
    double subtotalCost;
    double subtotalPrice;
    QString productName;
    QString productUom;
    StockUpdateDetail() : id(0), productId(0), quantity(0), cost(0.), price(0.), subtotalCost(0.), subtotalPrice(0.) {}
};

typedef QList<User> Users;
typedef QList<ProductCategory> ProductCategories;
typedef QList<Product> Products;
typedef QList<StockUpdate> SalesOrders;
typedef QList<StockUpdate> PurchaseOrders;
typedef QList<StockUpdateDetail> PurchaseOrderDetails;
typedef QList<StockUpdateDetail> SalesOrderDetails;
typedef QList<Party> Parties;
typedef QList<Customer> Customers;
typedef QList<Supplier> Suppliers;

#endif // COMMON_H
