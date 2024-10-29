#ifndef PRODUCTCATEGORYEDITOR_H
#define PRODUCTCATEGORYEDITOR_H

#include <QDialog>
#include "common.h"

namespace Ui {
class ProductCategoryEditor;
}

class ProductCategoryEditor : public QDialog
{
    Q_OBJECT

public:
    explicit ProductCategoryEditor(QWidget *parent = nullptr);
    ~ProductCategoryEditor();

    void edit(const ProductCategory &cat);

    QString name() const;

public slots:
    void accept();

private:
    Ui::ProductCategoryEditor *ui;
    ProductCategory item;
};

#endif // PRODUCTCATEGORYEDITOR_H
