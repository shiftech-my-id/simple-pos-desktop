#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QTabWidget;
class QPrinter;
class QLabel;
class QProcess;

namespace Ui {
class MainWindow;
}

class CustomerManager;
class SupplierManager;
class ProductManager;
class SalesOrderManager;
class PurchaseOrderManager;
class UserManager;

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
    void showUserManager();
    void showCustomerManager();
    void showSupplierManager();
    void showProductManager();
    void showProductCategoryManager();
    void showSalesOrderManager();
    void showPurchaseOrderManager();
    void showCalculatorDialog();
    void showChangePasswordDialog();

    void closeTab(int index);
    void printReportSalesDetailDaily(QPrinter *printer = nullptr);
    void printReportSalesDetail(QPrinter *printer = nullptr);
    void printReportMonthlySales(QPrinter *printer = nullptr);

    void updateDatabaseInfoLabel();
    void updateUsernameInfoLabel();
    void autoLogin();

private slots:
    void reloadStyleSheet();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;
    QProcess* calculatorProcess;
    QTabWidget *tabWidget;
    QWidget* dashboard;
    UserManager *userManager;
    CustomerManager* customerManager;
    SupplierManager* supplierManager;
    ProductManager *productManager;
    SalesOrderManager *salesOrderManager;
    PurchaseOrderManager *purchaseOrderManager;
    QLabel* usernameLabel;
    QLabel* databaseInfoLabel;
};

#endif // MAINWINDOW_H
