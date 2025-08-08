#pragma once

#include <QMainWindow>
#include <QTreeView>
#include <QStandardItemModel>
#include "database.h"
#include "cache.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_loadButton_clicked();
    void on_addButton_clicked();
    void on_deleteButton_clicked();
    void on_saveButton_clicked();
    void on_resetButton_clicked();

private:
    Ui::MainWindow *ui;
    Database db;
    Cache cache;

    QStandardItemModel* dbModel;
    QStandardItemModel* cacheModel;

    void populateModel(QStandardItem* parentItem, std::shared_ptr<TreeNode> node);
    void refreshViews();
    QString getSelectedId(QTreeView* view);
};
