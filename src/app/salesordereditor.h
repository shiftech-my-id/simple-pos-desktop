#ifndef SALESORDEREDITOR_H
#define SALESORDEREDITOR_H

#include <QDialog>
#include "common.h"
namespace Ui {
class SalesOrderEditor;
}

class QPrinter;
class SalesOrderEditorModel;
class SalesOrderItemEditor;

class SalesOrderEditor : public QDialog
{
    Q_OBJECT

public:
    explicit SalesOrderEditor(QWidget *parent = nullptr);
    ~SalesOrderEditor();

public slots:
    void add();
    void remove();
    bool save();
    void saveAndPrint(bool showPreview = true);
    void printReceipt(QPrinter *printer);
    void focusToDiscount();
    void focusToPayment();

    void accept();
    void reject();
    void load(const StockUpdate &order);

private slots:
    void onModelChanged();
    void updateTotal();
    void updateInfoLabel();
    void updateExchange();

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    void fixNumber(QObject *sender);
    void lockUi();

private:
    Ui::SalesOrderEditor *ui;
    SalesOrderEditorModel *model;
    StockUpdate salesOrder;
    SalesOrderItemEditor *editor;
    QAction *saveAction;
    QAction *removeAction;
};

#endif // SALESORDEREDITOR_H
