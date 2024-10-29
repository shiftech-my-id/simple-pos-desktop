#ifndef PRODUCTCATEGORYMANAGER_H
#define PRODUCTCATEGORYMANAGER_H

#include "common.h"
#include <QDialog>

namespace Ui {
class ProductCategoryManager;
}

class ProductCategoryModel;

class ProductCategoryManager : public QDialog
{
    Q_OBJECT

public:
    explicit ProductCategoryManager(QWidget *parent = nullptr);
    ~ProductCategoryManager();

    ProductCategory currentItem() const;
public slots:
    void refresh();
    void add();
    void edit();
    void remove();
    void updateButtonState();

private:
    Ui::ProductCategoryManager *ui;
    ProductCategoryModel *model;

    QAction* refreshAction;
    QAction* addAction;
    QAction* deleteAction;
};

#endif // PRODUCTCATEGORYMANAGER_H