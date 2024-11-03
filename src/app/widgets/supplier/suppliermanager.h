#ifndef SUPPLIERMANAGER_H
#define SUPPLIERMANAGER_H

#include <QWidget>

namespace Ui {
class SupplierManager;
}
class SupplierModel;
class SupplierProxyModel;
class Supplier;

class SupplierManager : public QWidget
{
    Q_OBJECT

public:
    explicit SupplierManager(QWidget *parent = nullptr);
    ~SupplierManager();

    Supplier currentItem() const;

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
    Ui::SupplierManager *ui;
    SupplierModel *model;
    SupplierProxyModel *proxyModel;

    QAction* refreshAction;
    QAction* addAction;
    QAction* duplicateAction;
    QAction* deleteAction;
};

#endif // SUPPLIERMANAGER_H
