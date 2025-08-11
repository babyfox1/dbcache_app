#pragma once

#include "treenode.h"
#include <memory>
#include <QString>

class Database {
public:
    Database();
    std::shared_ptr<TreeNode> getNode(const QString& id);
    void saveNode(const std::shared_ptr<TreeNode>& node);
    void reset();

    std::shared_ptr<TreeNode> getRoot();

private:
    std::shared_ptr<TreeNode> recursive_find(const std::shared_ptr<TreeNode>& node, const QString& id);
    void createTestData();
    std::shared_ptr<TreeNode> root;
};
