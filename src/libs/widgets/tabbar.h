#ifndef TABBAR_H
#define TABBAR_H

#include "global.h"
#include <QTabBar>
#include <QWidget>

class WIDGETS_EXPORT TabBar : public QTabBar
{
    Q_OBJECT

public:
    explicit TabBar(QWidget* parent = 0);

protected:
    QSize tabSizeHint(int size) const;
    void paintEvent(QPaintEvent *event);

};

#endif // TABBAR_H
