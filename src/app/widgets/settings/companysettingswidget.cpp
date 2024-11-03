#include "companysettingswidget.h"
#include "ui_companysettingswidget.h"
#include "db/settingtable.h"

CompanySettingsWidget::CompanySettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompanySettingsWidget)
{
    ui->setupUi(this);

    ui->nameEdit->setText(db::SettingTable::get("company.name").toString());
    ui->address1Edit->setText(db::SettingTable::get("company.address1").toString());
    ui->address2Edit->setText(db::SettingTable::get("company.address2").toString());
    ui->cityEdit->setText(db::SettingTable::get("company.city").toString());
    ui->provinceEdit->setText(db::SettingTable::get("company.province").toString());
    ui->postalCodeEdit->setText(db::SettingTable::get("company.postal_code").toString());
}

CompanySettingsWidget::~CompanySettingsWidget()
{
    delete ui;
}

void CompanySettingsWidget::save()
{
    QSqlDatabase db = db::connection();
    db.transaction();
    db::SettingTable::set("company.name", ui->nameEdit->text().trimmed());
    db::SettingTable::set("company.address1", ui->address1Edit->text().trimmed());
    db::SettingTable::set("company.address2", ui->address2Edit->text().trimmed());
    db::SettingTable::set("company.city", ui->cityEdit->text().trimmed());
    db::SettingTable::set("company.province", ui->provinceEdit->text().trimmed());
    db::SettingTable::set("company.postal_code", ui->postalCodeEdit->text().trimmed());
    db.commit();
}
