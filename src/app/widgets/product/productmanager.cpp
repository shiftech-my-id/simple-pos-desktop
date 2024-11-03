#include "db/db.h"
#include "global.h"
#include "productmanager.h"
#include "ui_productmanager.h"
#include "gui/productmodel.h"
#include "gui/productproxymodel.h"
#include "producteditor.h"
#include "widgets/application.h"
#include "common.h"

#include <QSqlQuery>
#include <QMessageBox>
#include <QToolBar>

ProductManager::ProductManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProductManager),
    model(qApp->productModel()),
    proxyModel(new ProductProxyModel(this))
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
    duplicateAction = toolBar->addAction(FA_ICON("fa-solid fa-copy"), "&Duplikat");
    deleteAction = toolBar->addAction(FA_ICON("fa-solid fa-trash"), "&Hapus");

    connect(refreshAction, &QAction::triggered, this, &ProductManager::refresh);
    connect(addAction, &QAction::triggered, this, &ProductManager::add);
    connect(deleteAction, &QAction::triggered, this, &ProductManager::remove);
    connect(duplicateAction, &QAction::triggered, this, &ProductManager::duplicate);

    ui->typeComboBox->addItem("Semua Jenis", -1);
    QMap<Product::Type, QString>::iterator i;
    for (i = Product::typeNames.begin(); i != Product::typeNames.end(); ++i) {
        ui->typeComboBox->addItem(i.value(), i.key());
    }
    ui->typeComboBox->setCurrentIndex(0);

    connect(ui->view, SIGNAL(activated(QModelIndex)), SLOT(edit()));
    connect(ui->searchEdit, &QLineEdit::textEdited, this, &ProductManager::filter);
    connect(ui->view->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), SLOT(updateButtonState()));
    connect(ui->showInactiveCheckBox, &QCheckBox::stateChanged, this, &ProductManager::filter);
    connect(ui->typeComboBox, &QComboBox::currentTextChanged, this, &ProductManager::filter);

    refresh();
}

ProductManager::~ProductManager()
{
    delete ui;
}

void ProductManager::add()
{
    ProductEditor editor(this);
    editor.setWindowTitle("Tambah Produk");
    if (!editor.exec())
        return;
    refresh();
}

void ProductManager::edit()
{
    Product item = currentItem();

    if (item.id == 0) {
        QMessageBox::information(this, "Informasi", "Silahkan pilih rekaman yang akan diubah.");
        return;
    }

    ProductEditor editor(this);
    editor.edit(item);
    if (!editor.exec())
        return;
    refresh();
}

void ProductManager::duplicate()
{
    Product item = currentItem();

    if (item.id == 0) {
        QMessageBox::information(this, "Informasi", "Silahkan pilih rekaman yang akan diduplikat.");
        return;
    }

    ProductEditor editor(this);
    editor.duplicate(item);
    if (!editor.exec())
        return;
    refresh();
}

void ProductManager::remove()
{
    Product item = currentItem();
    if (item.id == 0) {
        QMessageBox::information(this, "Informasi", "Silahkan pilih rekaman yang akan dihapus.");
        return;
    }

    if (msgBoxQuestion(this, "Konfimasi", "Hapus rekaman produk yang sedangdipilih?") != QMessageBox::Yes)
        return;

    QSqlQuery q(db::database());
    q.prepare("delete from products where id=:id");
    q.bindValue(":id", item.id);
    DB_EXEC(q);

    refresh();
}

void ProductManager::refresh()
{
    model->refresh();
    filter();
}

void ProductManager::filter()
{
    proxyModel->searchText = ui->searchEdit->text().trimmed();
    proxyModel->showInactive = ui->showInactiveCheckBox->isChecked();
    proxyModel->type = ui->typeComboBox->currentData().toInt();
    proxyModel->invalidate();
    ui->infoLabel->setText(QString("Menampilkan %1 produk dari total %2 produk.").arg(
        locale().toString(proxyModel->rowCount()),
        locale().toString(model->rowCount())
    ));
    updateButtonState();
    ui->view->horizontalHeader()->setSectionResizeMode(ui->view->horizontalHeader()->count() - 1, QHeaderView::Stretch);
}

Product ProductManager::currentItem() const
{
    QModelIndex index = ui->view->currentIndex();
    if (index.isValid())
        return model->items.at(proxyModel->mapToSource(index).row());

    return Product();
}

void ProductManager::updateButtonState()
{
    bool hasSelection = ui->view->selectionModel()->hasSelection();
    deleteAction->setEnabled(hasSelection);
    duplicateAction->setEnabled(hasSelection);
}

