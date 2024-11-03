#ifndef USEREDITOR_H
#define USEREDITOR_H

#include <QDialog>
#include "entity/user.h"

namespace Ui {
class UserEditor;
}

class UserEditor : public QDialog
{
    Q_OBJECT

public:
    User item;

    explicit UserEditor(QWidget *parent = nullptr);
    ~UserEditor();

    void edit(const User &item);

public slots:
    void accept();

private:
    Ui::UserEditor *ui;
};

#endif // USEREDITOR_H
