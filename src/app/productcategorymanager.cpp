#include "productcategorymanager.h"
#include "ui_productcategorymanager.h"
#include "productcategoryeditor.h"
#include "productcategorymodel.h"
#include "productcategoryproxymodel.h"
#include "db.h"
#include "global.h"
#include "application.h"

#include <QMessageBox>
#include <QToolBar>

ProductCategoryManager::ProductCategoryManager(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::ProductCategoryManager),
    model(qApp->productCategoryModel()),
    proxyModel(new ProductCategoryProxyModel(this))
{
    ui->setupUi(this);

    proxyModel->setSourceModel(model);
    ui->tableView->setModel(proxyModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    QToolBar *toolBar = new QToolBar(this);
    toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolBar->setIconSize(QSize(APP_TOOLBAR_ICON_SIZE, APP_TOOLBAR_ICON_SIZE));
    ui->toolBarLayout->insertWidget(0, toolBar);

    refreshAction = toolBar->addAction(FA_ICON("fa-solid fa-arrows-rotate"), "&Refresh");
    addAction = toolBar->addAction(FA_ICON("fa-solid fa-plus"), "&Tambah");
    deleteAction = toolBar->addAction(FA_ICON("fa-solid fa-trash"), "&Hapus");

    connect(ui->tableView, SIGNAL(activated(QModelIndex)), SLOT(edit()));
    connect(ui->tableView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), SLOT(updateButtonState()));
    connect(refreshAction, &QAction::triggered, this, &ProductCategoryManager::refresh);
    connect(addAction, &QAction::triggered, this, &ProductCategoryManager::add);
    connect(deleteAction, &QAction::triggered, this, &ProductCategoryManager::remove);
    connect(ui->searchEdit, &QLineEdit::textEdited, this, &ProductCategoryManager::filter);
}

ProductCategoryManager::~ProductCategoryManager()
{
    delete ui;
}

void ProductCategoryManager::add()
{
    ProductCategoryEditor editor(this);
    editor.setWindowTitle("Tambah Kategori Produk");
    if (!editor.exec())
        return;
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
}

void ProductCategoryManager::refresh()
{
    model->refresh();
    filter();
}

void ProductCategoryManager::filter()
{
    proxyModel->searchText = ui->searchEdit->text().trimmed();
    proxyModel->invalidate();
    ui->infoLabel->setText(QString("Menampilkan %1 kategori dari total %2 kategori.")
                               .arg(locale().toString(proxyModel->rowCount()))
                               .arg(locale().toString(model->rowCount())));
    updateButtonState();
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
}


ProductCategory ProductCategoryManager::currentItem() const
{
    QModelIndex index = ui->tableView->currentIndex();
    if (index.isValid())
        return model->items.at(proxyModel->mapToSource(index).row());

    return ProductCategory();
}

void ProductCategoryManager::updateButtonState()
{
    bool hasSelection = ui->tableView->selectionModel()->hasSelection();
    deleteAction->setEnabled(hasSelection);
}
