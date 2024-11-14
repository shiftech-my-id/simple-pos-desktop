#ifndef PRODUCTEDITOR_H
#define PRODUCTEDITOR_H

#include <QDialog>
#include "entity/product.h"


namespace Ui {
class ProductEditor;
}

class ProductCategoryModel;
class ProductCategoryProxyModel;

class ProductEditor : public QDialog
{
    Q_OBJECT

public:
    Product item;

    explicit ProductEditor(QWidget *parent = nullptr);
    ~ProductEditor();

    void edit(const Product &item);
    void duplicate(const Product &item);

public slots:
    void addCategory();
    void accept();

private:
    Ui::ProductEditor *ui;
    ProductCategoryModel *categoryModel;
    ProductCategoryProxyModel *categoryProxyModel;
};

#endif // PRODUCTEDITOR_H
