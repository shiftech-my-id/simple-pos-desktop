#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QTabWidget;
class QPrinter;

namespace Ui {
class MainWindow;
}

class ProductManager;
class ProductCategoryManager;
class SalesOrderManager;
class PurchaseOrderManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void showLoginDialog();
    void showAboutDialog();
    void showSettings();
    void showProductManager();
    void showProductCategoryManager();
    void showSalesOrderManager();
    void showPurchaseOrderManager();
    void showCalculatorDialog();

    void closeTab(int index);
    void printReportSalesDetailDaily(QPrinter *printer = nullptr);
    void printReportSalesDetail(QPrinter *printer = nullptr);
    void printReportMonthlySales(QPrinter *printer = nullptr);

private:
    Ui::MainWindow *ui;
    QTabWidget *tabWidget;
    QWidget* dashboard;
    ProductManager *productManager;
    ProductCategoryManager *productCategoryManager;
    SalesOrderManager *salesOrderManager;
    PurchaseOrderManager *purchaseOrderManager;
};

#endif // MAINWINDOW_H