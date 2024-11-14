#ifndef PURCHASEORDERITEMEDITOR_H
#define PURCHASEORDERITEMEDITOR_H

#include <QDialog>
#include "entity/product.h"
#include "entity/stockupdatedetail.h"
#include <QMap>

namespace Ui {
class PurchaseOrderItemEditor;
}

class PurchaseOrderItemEditor : public QDialog
{
    Q_OBJECT

public:
    explicit PurchaseOrderItemEditor(QWidget *parent = nullptr);
    ~PurchaseOrderItemEditor();

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
    Ui::PurchaseOrderItemEditor *ui;
    Product currentProduct;
    StockUpdateDetail item;

public:
    QMap<int, Product> productsByIds;
};

#endif // PURCHASEORDERITEMEDITOR_H
