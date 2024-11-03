#ifndef REPORTDATEDIALOG_H
#define REPORTDATEDIALOG_H

#include <QDialog>

namespace Ui {
class ReportDateDialog;
}

class ReportDateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReportDateDialog(QWidget *parent = nullptr);
    ~ReportDateDialog();

    int year() const;
    int month() const;

private:
    Ui::ReportDateDialog *ui;
};

#endif // REPORTDATEDIALOG_H
