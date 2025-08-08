#pragma once

#include "treenode.h"
#include <memory>
#include <unordered_map>
#include <QString>

class Database {
public:
    Database();
    std::shared_ptr<TreeNode> getNode(const QString& id);
    void saveNode(const std::shared_ptr<TreeNode>& node);
    void reset();

    std::shared_ptr<TreeNode> getRoot();

private:
    void createTestData();

    std::unordered_map<QString, std::shared_ptr<TreeNode>> storage;
    std::shared_ptr<TreeNode> root;
};
