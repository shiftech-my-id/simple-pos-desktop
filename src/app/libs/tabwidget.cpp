#include "tabwidget.h"
#include "tabbar.h"

TabWidget::TabWidget(QWidget* parent)
    : QTabWidget(parent)
{
    setTabBar(new TabBar(this));
}
