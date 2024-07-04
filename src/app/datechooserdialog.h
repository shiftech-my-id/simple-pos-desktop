#ifndef DATECHOOSERDIALOG_H
#define DATECHOOSERDIALOG_H

#include <QDialog>
#include <QDate>

namespace Ui {
class DateChooserDialog;
}

class DateChooserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DateChooserDialog(QWidget *parent = nullptr);
    ~DateChooserDialog();

    QDate selectedDate() const;

private:
    Ui::DateChooserDialog *ui;
};

#endif // DATECHOOSERDIALOG_H
