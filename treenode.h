#pragma once

#include <QString>
#include <QVector>
#include <memory>

struct TreeNode {
    QString id;
    QString data;
    bool isDeleted = false;
    std::weak_ptr<TreeNode> parent;
    QVector<std::shared_ptr<TreeNode>> children;

    TreeNode(const QString& id, const QString& data, std::shared_ptr<TreeNode> parent = nullptr);
};
