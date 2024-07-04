#include "productcategorymanager.h"
#include "ui_productcategorymanager.h"
#include "productcategoryeditor.h"
#include "productcategorymodel.h"
#include "db.h"
#include <QMessageBox>
#include <QToolBar>

ProductCategoryManager::ProductCategoryManager(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::ProductCategoryManager),
    model(new ProductCategoryModel(this))
{
    ui->setupUi(this);

    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    QToolBar *toolBar = new QToolBar(this);
    ui->toolBarLayout->insertWidget(0, toolBar);

    refreshAction = toolBar->addAction(QIcon(QString::fromUtf8(":/icons/sync")), "&Refresh");
    addAction = toolBar->addAction(QIcon(QString::fromUtf8(":/icons/add")), "&Tambah");
    deleteAction = toolBar->addAction(QIcon(QString::fromUtf8(":/icons/delete_forever")), "&Hapus");

    connect(ui->tableView, SIGNAL(activated(QModelIndex)), SLOT(edit()));
    connect(ui->tableView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
            SLOT(updateButtonState()));
    connect(refreshAction, &QAction::triggered, this, &ProductCategoryManager::refresh);
    connect(addAction, &QAction::triggered, this, &ProductCategoryManager::add);
    connect(deleteAction, &QAction::triggered, this, &ProductCategoryManager::remove);

    refresh();
}

ProductCategoryManager::~ProductCategoryManager()
{
    delete ui;
}


void ProductCategoryManager::add()
{
    ProductCategoryEditor editor(this);
    editor.setWindowTitle("Tambah Produk");
    if (!editor.exec())
        return;
    refresh();
}

void ProductCategoryManager::edit()
{
    ProductCategory item = currentItem();

    if (item.id == 0) {
        QMessageBox::information(this, "Informasi", "Silahkan pilih rekaman yang akan diubah.");
        return;
    }

    ProductCategoryEditor editor(this);
    editor.edit(item);
    if (!editor.exec())
        return;
    refresh();
}

void ProductCategoryManager::remove()
{
    ProductCategory item = currentItem();
    if (item.id == 0) {
        QMessageBox::information(this, "Informasi", "Silahkan pilih rekaman yang akan dihapus.");
        return;
    }

    if (msgBoxQuestion(this, "Konfimasi", QString("Hapus kategori <b>%1</b>?").arg(item.name)) != QMessageBox::Yes)
        return;

    QSqlQuery q(db::database());
    q.prepare("delete from product_categories where id=:id");
    q.bindValue(":id", item.id);
    DB_EXEC(q);

    refresh();
}

void ProductCategoryManager::refresh()
{
    model->refresh();

    updateButtonState();
}


ProductCategory ProductCategoryManager::currentItem() const
{
    QModelIndex index = ui->tableView->currentIndex();
    if (index.isValid())
        return model->items.at(index.row());

    return ProductCategory();
}

void ProductCategoryManager::updateButtonState()
{
    bool hasSelection = ui->tableView->selectionModel()->hasSelection();
    deleteAction->setEnabled(hasSelection);
}
