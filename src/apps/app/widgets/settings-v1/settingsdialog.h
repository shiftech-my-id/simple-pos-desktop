#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class CompanySettingsWidget;

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

public slots:
    void accept();
private:
    Ui::SettingsDialog *ui;
    CompanySettingsWidget *companySettingsWidget;
};

#endif // SETTINGSDIALOG_H
