#ifndef PRODUCTEDITOR_H
#define PRODUCTEDITOR_H

#include <QDialog>
#include "common.h"


namespace Ui {
class ProductEditor;
}

class ProductEditor : public QDialog
{
    Q_OBJECT

public:
    Product item;

    explicit ProductEditor(QWidget *parent = nullptr);
    ~ProductEditor();

    void edit(const Product &item);
    void duplicate(const Product &item);

    void refreshCategories();

public slots:
    void addCategory();
    void accept();

private:
    Ui::ProductEditor *ui;
};

#endif // PRODUCTEDITOR_H
