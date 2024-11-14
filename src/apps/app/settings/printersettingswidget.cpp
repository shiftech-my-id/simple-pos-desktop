#include "printersettingswidget.h"
#include "ui_printersettingswidget.h"

PrinterSettingsWidget::PrinterSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrinterSettingsWidget)
{
    ui->setupUi(this);
}

PrinterSettingsWidget::~PrinterSettingsWidget()
{
    delete ui;
}
