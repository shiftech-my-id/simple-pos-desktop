#include "db.h"
#include "global.h"
#include "usermanager.h"
#include "ui_usermanager.h"
#include "usermodel.h"
#include "userproxymodel.h"
#include "usereditor.h"
#include "application.h"

#include <QSqlQuery>
#include <QMessageBox>
#include <QToolBar>

UserManager::UserManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserManager),
    model(qApp->userModel()),
    proxyModel(new UserProxyModel(this))
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

    connect(refreshAction, &QAction::triggered, this, &UserManager::refresh);
    connect(addAction, &QAction::triggered, this, &UserManager::add);
    connect(deleteAction, &QAction::triggered, this, &UserManager::remove);

    ui->roleComboBox->addItem("Semua Role", -1);
    QMap<User::Role, QString>::iterator i;
    for (i = User::roleNames.begin(); i != User::roleNames.end(); ++i) {
        ui->roleComboBox->addItem(i.value(), i.key());
    }
    ui->roleComboBox->setCurrentIndex(0);

    connect(ui->view, SIGNAL(activated(QModelIndex)), SLOT(edit()));
    connect(ui->searchEdit, &QLineEdit::textEdited, this, &UserManager::filter);
    connect(ui->view->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), SLOT(updateButtonState()));
    connect(ui->showInactiveCheckBox, &QCheckBox::stateChanged, this, &UserManager::filter);
    connect(ui->roleComboBox, &QComboBox::currentTextChanged, this, &UserManager::filter);

    refresh();
}

UserManager::~UserManager()
{
    delete ui;
}

void UserManager::add()
{
    UserEditor editor(this);
    editor.setWindowTitle("Tambah Pengguna");
    if (!editor.exec())
        return;
    refresh();
}

void UserManager::edit()
{
    User item = currentItem();

    if (item.id == 0) {
        QMessageBox::information(this, "Informasi", "Silahkan pilih rekaman yang akan diubah.");
        return;
    }

    UserEditor editor(this);
    editor.edit(item);
    if (!editor.exec())
        return;
    refresh();
}

void UserManager::remove()
{
    User item = currentItem();
    if (item.id == 0) {
        QMessageBox::information(this, "Informasi", "Silahkan pilih rekaman yang akan dihapus.");
        return;
    }

    if (msgBoxQuestion(this, "Konfimasi", "Hapus rekaman pengguna yang sedangdipilih?") != QMessageBox::Yes)
        return;

    QSqlQuery q(db::database());
    q.prepare("delete from users where id=:id");
    q.bindValue(":id", item.id);
    DB_EXEC(q);

    refresh();
}

void UserManager::refresh()
{
    model->refresh();
    filter();
}

void UserManager::filter()
{
    proxyModel->searchText = ui->searchEdit->text().trimmed();
    proxyModel->showInactive = ui->showInactiveCheckBox->isChecked();
    proxyModel->role = ui->roleComboBox->currentData().toInt();
    proxyModel->invalidate();
    ui->infoLabel->setText(QString("Menampilkan %1 pengguna dari total %2 pengguna.").arg(
        locale().toString(proxyModel->rowCount()),
        locale().toString(model->rowCount())
    ));
    updateButtonState();
    ui->view->horizontalHeader()->setSectionResizeMode(ui->view->horizontalHeader()->count() - 1, QHeaderView::Stretch);
}

User UserManager::currentItem() const
{
    QModelIndex index = ui->view->currentIndex();
    if (index.isValid())
        return model->items.at(proxyModel->mapToSource(index).row());

    return User();
}

void UserManager::updateButtonState()
{
    bool hasSelection = ui->view->selectionModel()->hasSelection();
    deleteAction->setEnabled(hasSelection);
}

