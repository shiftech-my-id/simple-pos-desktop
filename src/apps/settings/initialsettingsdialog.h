#ifndef INITIALSETTINGSDIALOG_H
#define INITIALSETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class InitialSettingsDialog;
}

class InitialSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InitialSettingsDialog(QWidget *parent = 0);
    ~InitialSettingsDialog();

public slots:
    void saveAndClose();
    void testDatabaseConnection();

private slots:
    void onDriverChanged();

private:
    Ui::InitialSettingsDialog *ui;
};

#endif // INITIALSETTINGSDIALOG_H
