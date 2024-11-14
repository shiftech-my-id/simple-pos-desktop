#ifndef COMPANYSETTINGSWIDGET_H
#define COMPANYSETTINGSWIDGET_H

#include <QWidget>

namespace Ui {
class CompanySettingsWidget;
}

class CompanySettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CompanySettingsWidget(QWidget *parent = nullptr);
    ~CompanySettingsWidget();

    void save();

private:
    Ui::CompanySettingsWidget *ui;
};

#endif // COMPANYSETTINGSWIDGET_H
