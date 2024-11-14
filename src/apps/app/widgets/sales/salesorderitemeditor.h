#ifndef SALESORDERITEMEDITOR_H
#define SALESORDERITEMEDITOR_H

#include <QDialog>
#include "entity/stockupdatedetail.h"
#include "entity/product.h"
#include <QMap>

namespace Ui {
class SalesOrderItemEditor;
}

class SalesOrderItemEditor : public QDialog
{
    Q_OBJECT

public:
    explicit SalesOrderItemEditor(QWidget *parent = nullptr);
    ~SalesOrderItemEditor();

signals:
    void itemAdded(const StockUpdateDetail &item);

private slots:
    void updateUi();
    void addItem();
    void onProductChanged();
    void addNewProduct();
    void refreshProducts();

protected:
    bool eventFilter(QObject *o, QEvent* e);

private:
    Ui::SalesOrderItemEditor *ui;
    Product currentProduct;
    StockUpdateDetail item;

public:
    QMap<int, Product> productsByIds;
};

#endif // SALESORDERITEMEDITOR_H
