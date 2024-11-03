#include "tableview.h"
#include <QHeaderView>

TableView::TableView(QWidget *parent)
    : QTableView(parent)
{
    setAlternatingRowColors(true);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSortingEnabled(true);
    setCornerButtonEnabled(false);
    verticalHeader()->setVisible(false);
    verticalHeader()->setMinimumSectionSize(30);
    verticalHeader()->setDefaultSectionSize(30);
    verticalHeader()->setHighlightSections(false);
    horizontalHeader()->setHighlightSections(false);
}
