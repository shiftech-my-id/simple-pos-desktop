#ifndef TABWIDGET_H
#define TABWIDGET_H

#include "global.h"
#include <QTabWidget>
#include <QWidget>

class WIDGETS_EXPORT TabWidget : public QTabWidget
{
    Q_OBJECT

public:
    TabWidget(QWidget* parent = 0);
};

#endif // TABWIDGET_H
