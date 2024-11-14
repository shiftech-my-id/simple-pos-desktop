#include "lefttabwidget.h"
#include "ui_lefttabwidget.h"

#include <QToolButton>
#include <QButtonGroup>

LeftTabWidget::LeftTabWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LeftTabWidget)
{
    ui->setupUi(this);

    buttonGroup = new QButtonGroup(this);
    ui->buttonFrame->setMaximumWidth(100);
}

LeftTabWidget::~LeftTabWidget()
{
    delete ui;
}

int LeftTabWidget::addWidget(const QString& label, const QIcon& icon, QWidget* widget)
{
    QToolButton* button = new QToolButton(widget);
    button->setFixedHeight(60);
    button->setText(label);
    button->setIcon(icon);
    button->setIconSize(QSize(24, 24));
    button->setCheckable(true);
    button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    button->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    connect(button, SIGNAL(toggled(bool)), SLOT(onButtonToggled(bool)));

    int i = ui->stackedWidget->addWidget(widget);
    button->setProperty("index", i);
    ui->buttonLayout->insertWidget(i, button);
    buttonGroup->addButton(button, i);

    return i;
}

void LeftTabWidget::onButtonToggled(bool active)
{
    if (!active)
        return;

    ui->stackedWidget->setCurrentIndex(sender()->property("index").toInt());
}

void LeftTabWidget::setCurrentIndex(int i)
{
    buttonGroup->button(i)->setChecked(true);
}
