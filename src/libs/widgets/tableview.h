#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include "global.h"
#include <QTableView>

class WIDGETS_EXPORT TableView : public QTableView
{
    Q_OBJECT

public:
    TableView(QWidget *parent = nullptr);
};

#endif // TABLEVIEW_H