#include "db.h"
#include "productmanager.h"
#include "ui_productmanager.h"
#include "productmodel.h"
#include "productproxymodel.h"
#include "producteditor.h"

#include <QSqlQuery>
#include <QMessageBox>
#include <QToolBar>

ProductManager::ProductManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProductManager),
    model(new ProductModel(this)),
    proxyModel(new ProductProxyModel(this))
{
    ui->setupUi(this);
    proxyModel->setSourceModel(model);
    ui->view->setModel(proxyModel);

    QToolBar *toolBar = new QToolBar(this);
    ui->toolBarLayout->insertWidget(0, toolBar);

    refreshAction = toolBar->addAction(QIcon(QString::fromUtf8(":/icons/sync")), "&Refresh");
    addAction = toolBar->addAction(QIcon(QString::fromUtf8(":/icons/add")), "&Tambah");
    deleteAction = toolBar->addAction(QIcon(QString::fromUtf8(":/icons/delete_forever")), "&Hapus");
    connect(refreshAction, &QAction::triggered, this, &ProductManager::refresh);
    connect(addAction, &QAction::triggered, this, &ProductManager::add);
    connect(deleteAction, &QAction::triggered, this, &ProductManager::remove);

    ui->typeComboBox->addItem("Semua Jenis", -1);
    QMap<Product::Type, QString>::iterator i;
    for (i = Product::typeNames.begin(); i != Product::typeNames.end(); ++i) {
        ui->typeComboBox->addItem(i.value(), i.key());
    }
    ui->typeComboBox->setCurrentIndex(0);

    connect(ui->view, SIGNAL(activated(QModelIndex)), SLOT(edit()));
    connect(ui->searchEdit, SIGNAL(editingFinished()), SLOT(filter()));
    connect(ui->view->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
            SLOT(updateButtonState()));
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
    ui->infoLabel->setText(QString("Menampilkan %1 produk dari total %2 produk.")
                           .arg(locale().toString(proxyModel->rowCount()))
                           .arg(locale().toString(model->rowCount())));
    updateButtonState();
    ui->view->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
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
}

