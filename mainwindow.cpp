#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItem>
#include <QInputDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    dbModel = new QStandardItemModel(this);
    cacheModel = new QStandardItemModel(this);

    ui->dbTreeView->setModel(dbModel);
    ui->cacheTreeView->setModel(cacheModel);

    refreshViews();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::populateModel(QStandardItem* parentItem, std::shared_ptr<TreeNode> node) {
    if (!node) return;

    auto item = new QStandardItem(node->data);
    item->setData(node->id, Qt::UserRole + 1);

    if (node->isDeleted) {
        item->setForeground(Qt::gray);
        item->setText(item->text() + " (deleted)");
    }

    parentItem->appendRow(item);

    for (auto& child : node->children) {
        populateModel(item, child);
    }
}

void MainWindow::refreshViews() {
    dbModel->clear();
    cacheModel->clear();

    auto dbRoot = db.getRoot();
    auto cacheRoot = cache.getRoot();

    auto dbItem = new QStandardItem("DB Root");
    dbModel->appendRow(dbItem);
    populateModel(dbItem, dbRoot);

    auto cacheItem = new QStandardItem("Cache Root");
    cacheModel->appendRow(cacheItem);
    populateModel(cacheItem, cacheRoot);
}

QString MainWindow::getSelectedId(QTreeView* view) {
    auto index = view->currentIndex();
    if (!index.isValid()) return "";
    return index.data(Qt::UserRole + 1).toString();
}

void MainWindow::on_loadButton_clicked() {
    auto node = db.getRoot();
    cache.loadFromDb(node);
    refreshViews();

    ui->cacheTreeView->expandAll();
}

void MainWindow::on_addButton_clicked() {
    QString id = getSelectedId(ui->cacheTreeView);
    if (id.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Не выбран элемент");
        return;
    }

    auto node = cache.getNode(id);
    if (!node) {
        QMessageBox::warning(this, "Ошибка", "Элемент не найден");
        return;
    }

    if (node->isDeleted) {
        QMessageBox::warning(this, "Ошибка",
                             "Нельзя добавить потомка к удалённому элементу: " + node->data);
        return;
    }

    bool ok;
    QString data = QInputDialog::getText(this, "Добавить потомка",
                                         "Введите данные для нового элемента:",
                                         QLineEdit::Normal, "", &ok);
    if (!ok || data.isEmpty()) return;

    cache.addChild(id, data);
    refreshViews();

    ui->cacheTreeView->expandAll();
}

void MainWindow::on_deleteButton_clicked() {
    QString id = getSelectedId(ui->cacheTreeView);
    if (id.isEmpty()) return;

    cache.markDeleted(id);
    refreshViews();

    ui->cacheTreeView->expandAll();
}

void MainWindow::on_saveButton_clicked() {
    cache.saveToDb(db);
    refreshViews();

    ui->dbTreeView->expandAll();
    ui->cacheTreeView->expandAll();

    QMessageBox::information(this, "Успех", "Изменения сохранены в БД");
}

void MainWindow::on_resetButton_clicked() {
    db.reset();
    cache.reset();
    refreshViews();
}
