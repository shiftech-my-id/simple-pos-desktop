#include "tabbar.h"
#include <QStylePainter>
#include <QStyleOptionTab>

TabBar::TabBar(QWidget* parent)
    : QTabBar(parent)
{
    // setIconSize(QSize(80, 80));
}

QSize TabBar::tabSizeHint(int) const
{
    return QSize(64, 64);
}

void TabBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QStylePainter p(this);

    for (int index = 0; index < count(); index++)
    {
        QStyleOptionTab tab;
        initStyleOption(&tab, index);

        QIcon tempIcon = tab.icon;
        QString tempText = tab.text;
        tab.icon = QIcon();
        tab.text = QString();

        p.drawControl(QStyle::CE_TabBarTab, tab);

        QPainter painter;
        painter.begin(this);

        QRect tabrect = tabRect(index);
        tabrect.adjust(0, 8, 0, -8);
        painter.drawText(tabrect, Qt::AlignBottom | Qt::AlignHCenter, tempText);
        tempIcon.paint(&painter, 0, tabrect.top(), tab.iconSize.width(), tab.iconSize.height(), Qt::AlignTop | Qt::AlignHCenter);
        painter.end();
    }
}
