#ifndef CUSTOMEREDITOR_H
#define CUSTOMEREDITOR_H

#include <QDialog>
#include "common.h"


namespace Ui {
class CustomerEditor;
}

class CustomerEditor : public QDialog
{
    Q_OBJECT

public:
    Customer item;

    explicit CustomerEditor(QWidget *parent = nullptr);
    ~CustomerEditor();

    void edit(const Customer &item);
    void duplicate(const Customer &item);

public slots:
    void accept();

private:
    Ui::CustomerEditor *ui;
};

#endif // CUSTOMEREDITOR_H
