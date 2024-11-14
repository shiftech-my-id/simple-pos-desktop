#ifndef PURCHASEORDEREDITOR_H
#define PURCHASEORDEREDITOR_H

#include <QDialog>
#include "entity/stockupdate.h"

namespace Ui {
class PurchaseOrderEditor;
}

class QPrinter;
class PurchaseOrderEditorModel;
class PurchaseOrderItemEditor;

class PurchaseOrderEditor : public QDialog
{
    Q_OBJECT

public:
    explicit PurchaseOrderEditor(QWidget *parent = nullptr);
    ~PurchaseOrderEditor();

public slots:
    void add();
    void remove();
    bool save();
    void saveAndPrint(bool showPreview = true);
    void printReceipt(QPrinter *printer);

    void accept();
    void reject();
    void load(const StockUpdate &order);

private slots:
    void onModelChanged();
    void updateInfoLabel();

private:
    void fixNumber(QObject *sender);
    void lockUi();

private:
    Ui::PurchaseOrderEditor *ui;
    PurchaseOrderEditorModel *model;
    StockUpdate purchaseOrder;
    PurchaseOrderItemEditor *editor;
    QAction *saveAction;
    QAction *removeAction;
};

#endif // PURCHASEORDEREDITOR_H
