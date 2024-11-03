#include "db/db.h"
#include "global.h"
#include "common.h"
#include "customermanager.h"
#include "ui_customermanager.h"
#include "gui/customermodel.h"
#include "gui/customerproxymodel.h"
#include "customereditor.h"
#include "widgets/application.h"

#include <QSqlQuery>
#include <QMessageBox>
#include <QToolBar>

CustomerManager::CustomerManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomerManager),
    model(qApp->customerModel()),
    proxyModel(new CustomerProxyModel(this))
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

    connect(refreshAction, &QAction::triggered, this, &CustomerManager::refresh);
    connect(addAction, &QAction::triggered, this, &CustomerManager::add);
    connect(deleteAction, &QAction::triggered, this, &CustomerManager::remove);
    connect(duplicateAction, &QAction::triggered, this, &CustomerManager::duplicate);

    connect(ui->view, SIGNAL(activated(QModelIndex)), SLOT(edit()));
    connect(ui->searchEdit, &QLineEdit::textEdited, this, &CustomerManager::filter);
    connect(ui->view->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), SLOT(updateButtonState()));
    connect(ui->showInactiveCheckBox, &QCheckBox::stateChanged, this, &CustomerManager::filter);

    refresh();
}

CustomerManager::~CustomerManager()
{
    delete ui;
}

void CustomerManager::add()
{
    CustomerEditor editor(this);
    editor.setWindowTitle("Tambah Pelanggan");
    if (!editor.exec())
        return;
    refresh();
}

void CustomerManager::edit()
{
    Customer item = currentItem();

    if (item.id == 0) {
        QMessageBox::information(this, "Informasi", "Silahkan pilih rekaman yang akan diubah.");
        return;
    }

    CustomerEditor editor(this);
    editor.edit(item);
    if (!editor.exec())
        return;
    refresh();
}

void CustomerManager::duplicate()
{
    Customer item = currentItem();

    if (item.id == 0) {
        QMessageBox::information(this, "Informasi", "Silahkan pilih rekaman yang akan diduplikat.");
        return;
    }

    CustomerEditor editor(this);
    editor.duplicate(item);
    if (!editor.exec())
        return;
    refresh();
}

void CustomerManager::remove()
{
    Customer item = currentItem();
    if (item.id == 0) {
        QMessageBox::information(this, "Informasi", "Silahkan pilih rekaman yang akan dihapus.");
        return;
    }

    if (msgBoxQuestion(this, "Konfimasi", "Hapus rekaman yang sedang dipilih?") != QMessageBox::Yes)
        return;

    QSqlQuery q(db::database());
    q.prepare("delete from parties where id=:id");
    q.bindValue(":id", item.id);
    DB_EXEC(q);

    refresh();
}

void CustomerManager::refresh()
{
    model->refresh();
    filter();
}

void CustomerManager::filter()
{
    proxyModel->searchText = ui->searchEdit->text().trimmed();
    proxyModel->showInactive = ui->showInactiveCheckBox->isChecked();
    proxyModel->invalidate();
    ui->infoLabel->setText(QString("Menampilkan %1 pelanggan dari total %2 pelanggan.").arg(
        locale().toString(proxyModel->rowCount()),
        locale().toString(model->rowCount())
    ));
    updateButtonState();
    ui->view->horizontalHeader()->setSectionResizeMode(ui->view->horizontalHeader()->count() - 1, QHeaderView::Stretch);
}

Customer CustomerManager::currentItem() const
{
    QModelIndex index = ui->view->currentIndex();
    if (index.isValid())
        return model->items.at(proxyModel->mapToSource(index).row());

    return Customer();
}

void CustomerManager::updateButtonState()
{
    bool hasSelection = ui->view->selectionModel()->hasSelection();
    deleteAction->setEnabled(hasSelection);
    duplicateAction->setEnabled(hasSelection);
}

