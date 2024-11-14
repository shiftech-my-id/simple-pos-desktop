#ifndef PURCHASEORDEREDITORMODEL_H
#define PURCHASEORDEREDITORMODEL_H

#include <QAbstractTableModel>
#include "entity/stockupdatedetail.h"

class PurchaseOrderEditorModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    bool readOnly;

    typedef StockUpdateDetail Item;
    typedef PurchaseOrderDetails Items;

    Items items;
    QMap<QString, int> itemIndexByNames;

    explicit PurchaseOrderEditorModel(QObject *parent = nullptr);

    Qt::ItemFlags flags(const QModelIndex &index) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    void removeRow(int row);

    void refresh(int orderId);

public slots:
    void addItem(const StockUpdateDetail &item);

private:
    void updateQuantity(int row, int quantity);

};

#endif // PURCHASEORDEREDITORMODEL_H
