#include "db.h"
#include "global.h"
#include "suppliermanager.h"
#include "ui_suppliermanager.h"
#include "suppliermodel.h"
#include "supplierproxymodel.h"
#include "suppliereditor.h"
#include "application.h"

#include <QSqlQuery>
#include <QMessageBox>
#include <QToolBar>

SupplierManager::SupplierManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SupplierManager),
    model(qApp->supplierModel()),
    proxyModel(new SupplierProxyModel(this))
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

    connect(refreshAction, &QAction::triggered, this, &SupplierManager::refresh);
    connect(addAction, &QAction::triggered, this, &SupplierManager::add);
    connect(deleteAction, &QAction::triggered, this, &SupplierManager::remove);
    connect(duplicateAction, &QAction::triggered, this, &SupplierManager::duplicate);

    connect(ui->view, SIGNAL(activated(QModelIndex)), SLOT(edit()));
    connect(ui->searchEdit, &QLineEdit::textEdited, this, &SupplierManager::filter);
    connect(ui->view->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), SLOT(updateButtonState()));
    connect(ui->showInactiveCheckBox, &QCheckBox::stateChanged, this, &SupplierManager::filter);

    refresh();
}

SupplierManager::~SupplierManager()
{
    delete ui;
}

void SupplierManager::add()
{
    SupplierEditor editor(this);
    editor.setWindowTitle("Tambah Supplier");
    if (!editor.exec())
        return;
    refresh();
}

void SupplierManager::edit()
{
    Supplier item = currentItem();

    if (item.id == 0) {
        QMessageBox::information(this, "Informasi", "Silahkan pilih rekaman yang akan diubah.");
        return;
    }

    SupplierEditor editor(this);
    editor.edit(item);
    if (!editor.exec())
        return;
    refresh();
}

void SupplierManager::duplicate()
{
    Supplier item = currentItem();

    if (item.id == 0) {
        QMessageBox::information(this, "Informasi", "Silahkan pilih rekaman yang akan diduplikat.");
        return;
    }

    SupplierEditor editor(this);
    editor.duplicate(item);
    if (!editor.exec())
        return;
    refresh();
}

void SupplierManager::remove()
{
    Supplier item = currentItem();
    if (item.id == 0) {
        QMessageBox::information(this, "Informasi", "Silahkan pilih rekaman yang akan dihapus.");
        return;
    }

    if (msgBoxQuestion(this, "Konfimasi", "Hapus rekaman supplier yang sedang dipilih?") != QMessageBox::Yes)
        return;

    QSqlQuery q(db::database());
    q.prepare("delete from parties where type=1 and id=:id");
    q.bindValue(":id", item.id);
    DB_EXEC(q);

    refresh();
}

void SupplierManager::refresh()
{
    model->refresh();
    filter();
}

void SupplierManager::filter()
{
    proxyModel->searchText = ui->searchEdit->text().trimmed();
    proxyModel->showInactive = ui->showInactiveCheckBox->isChecked();
    proxyModel->invalidate();
    ui->infoLabel->setText(QString("Menampilkan %1 supplier dari total %2 supplier.").arg(
        locale().toString(proxyModel->rowCount()),
        locale().toString(model->rowCount())
    ));
    updateButtonState();
    ui->view->horizontalHeader()->setSectionResizeMode(ui->view->horizontalHeader()->count() - 1, QHeaderView::Stretch);
}

Supplier SupplierManager::currentItem() const
{
    QModelIndex index = ui->view->currentIndex();
    if (index.isValid())
        return model->items.at(proxyModel->mapToSource(index).row());

    return Supplier();
}

void SupplierManager::updateButtonState()
{
    bool hasSelection = ui->view->selectionModel()->hasSelection();
    deleteAction->setEnabled(hasSelection);
    duplicateAction->setEnabled(hasSelection);
}

