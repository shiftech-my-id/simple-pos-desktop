#ifndef SUPPLIEREDITOR_H
#define SUPPLIEREDITOR_H

#include <QDialog>
#include "entity/supplier.h"

namespace Ui {
class SupplierEditor;
}

class SupplierEditor : public QDialog
{
    Q_OBJECT

public:
    Supplier item;

    explicit SupplierEditor(QWidget *parent = nullptr);
    ~SupplierEditor();

    QString name() const;

    void edit(const Supplier &item);
    void duplicate(const Supplier &item);

public slots:
    void accept();

private:
    Ui::SupplierEditor *ui;
};

#endif // SUPPLIEREDITOR_H
