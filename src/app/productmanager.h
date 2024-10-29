#ifndef PRODUCTMANAGER_H
#define PRODUCTMANAGER_H

#include <QWidget>

namespace Ui {
class ProductManager;
}
class ProductModel;
class ProductProxyModel;
class Product;

class ProductManager : public QWidget
{
    Q_OBJECT

public:
    explicit ProductManager(QWidget *parent = nullptr);
    ~ProductManager();

    Product currentItem() const;

public slots:
    void add();
    void edit();
    void duplicate();
    void remove();
    void refresh();
    void filter();

private slots:
    void updateButtonState();

private:
    Ui::ProductManager *ui;
    ProductModel *model;
    ProductProxyModel *proxyModel;

    QAction* refreshAction;
    QAction* addAction;
    QAction* duplicateAction;
    QAction* deleteAction;
};

#endif // PRODUCTMANAGER_H
