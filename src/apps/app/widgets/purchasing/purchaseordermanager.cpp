#include "purchaseordermanager.h"
#include "ui_purchaseordermanager.h"
#include "purchaseordereditor.h"
#include "gui/purchaseordermodel.h"
#include "gui/purchaseorderproxymodel.h"
#include "db/db.h"
#include "global.h"
#include "common.h"
#include "entity/product.h"
#include "widgets/application.h"
#include <QMessageBox>
#include <QToolBar>
#include <QIcon>

PurchaseOrderManager::PurchaseOrderManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PurchaseOrderManager),
    model(new PurchaseOrderModel(this)),
    proxyModel(new PurchaseOrderProxyModel(this))
{
    ui->setupUi(this);

    proxyModel->setSourceModel(model);
    ui->view->setModel(proxyModel);

    QToolBar *toolBar = new QToolBar(this);
    toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolBar->setIconSize(QSize(APP_TOOLBAR_ICON_SIZE, APP_TOOLBAR_ICON_SIZE));
    ui->toolBarLayout->insertWidget(0, toolBar);

    refreshAction = toolBar->addAction(FA_ICON("fa-solid fa-arrows-rotate"), "&Refresh");
    addAction = toolBar->addAction(FA_ICON("fa-solid fa-plus"), "&Tambah");
    deleteAction = toolBar->addAction(FA_ICON("fa-solid fa-trash"), "&Hapus");
    connect(refreshAction, &QAction::triggered, this, &PurchaseOrderManager::refresh);
    connect(addAction, &QAction::triggered, this, &PurchaseOrderManager::add);
    connect(deleteAction, &QAction::triggered, this, &PurchaseOrderManager::remove);

    connect(ui->view, SIGNAL(activated(QModelIndex)), SLOT(edit()));
    connect(ui->searchEdit, SIGNAL(editingFinished()), SLOT(filter()));
    connect(ui->view->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
            SLOT(updateButtonState()));
    connect(ui->dateComboBox, SIGNAL(currentIndexChanged(int)), SLOT(refresh()));

    refresh();
}

PurchaseOrderManager::~PurchaseOrderManager()
{
    delete ui;
}

void PurchaseOrderManager::add()
{
    PurchaseOrderEditor editor(this);
    editor.setMinimumWidth(800);
    editor.showMaximized();
    if (!editor.exec())
        return;

    emit productStockChanged();
    refresh();
}

void PurchaseOrderManager::edit()
{
    PurchaseOrderEditor editor(this);
    editor.setMinimumWidth(800);
    editor.showMaximized();
    editor.load(currentItem());
    if (!editor.exec())
        return;
}

void PurchaseOrderManager::remove()
{
    StockUpdate item = currentItem();
    if (item.id == 0) {
        QMessageBox::information(this, "Informasi", "Silahkan pilih rekaman yang akan dihapus.");
        return;
    }

    if (msgBoxQuestion(this, "Konfimasi", QString("Hapus rekaman penjualan <b>#%1</b>?").arg(item.id)) != QMessageBox::Yes) {
        return;
    }

    QSqlDatabase db = db::database();
    db.transaction();
    QSqlQuery q(db);
    q.prepare("delete from stock_updates where id=:id");
    q.bindValue(":id", item.id);
    DB_EXEC(q);

    q.prepare("select * from stock_update_details where update_id=:update_id");
    q.bindValue(":update_id", item.id);
    DB_EXEC(q);
    QSqlQuery q2(db);
    while (q.next()) {
        int productId = q.value("product_id").toInt();
        int quantity = q.value("quantity").toInt();
        q2.prepare("select * from products where id=:id");
        q2.bindValue(":id", productId);
        DB_EXEC(q2);

        if (q2.next()) {
            int productType = q2.value("type").toInt();
            if (productType == Product::Stocked) {
                int revertedStock = q2.value("stock").toInt() - quantity;
                QSqlQuery q3(db);
                q3.prepare("update products set stock=:stock where id=:id");
                q3.bindValue(":stock", revertedStock);
                q3.bindValue(":id", productId);
                DB_EXEC(q3);

                emit productStockChanged();
            }
        }
    }

    q.prepare("delete from stock_update_details where update_id=:update_id");
    q.bindValue(":update_id", item.id);
    DB_EXEC(q);

    db.commit();

    refresh();
}

void PurchaseOrderManager::refresh()
{
    model->refresh(ui->dateComboBox->currentText());
    filter();
}

void PurchaseOrderManager::filter()
{
    proxyModel->searchText = ui->searchEdit->text().trimmed();
    proxyModel->invalidate();
    ui->infoLabel->setText(QString("Menampilkan %1 rekaman dari total %2 rekaman.")
                           .arg(locale().toString(proxyModel->rowCount()), locale().toString(model->rowCount())));
    updateButtonState();
    ui->view->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
}

StockUpdate PurchaseOrderManager::currentItem() const
{
    QModelIndex index = ui->view->currentIndex();
    if (index.isValid())
        return model->items.at(proxyModel->mapToSource(index).row());

    return StockUpdate();
}

void PurchaseOrderManager::updateButtonState()
{
    bool hasSelection = ui->view->selectionModel()->hasSelection();
    deleteAction->setEnabled(hasSelection);
}
