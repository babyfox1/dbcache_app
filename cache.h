#pragma once

#include "treenode.h"
#include "database.h"
#include <memory>
#include <unordered_map>
#include <QString>

class Cache {
public:
    Cache();
    void loadFromDb(const std::shared_ptr<TreeNode>& dbNode);
    std::shared_ptr<TreeNode> getNode(const QString& id);
    bool isNodeDeleted(const QString& id) const;
    void addChild(const QString& parentId, const QString& data);
    void markDeleted(const QString& id);
    void saveToDb(Database& db);
    void reset();

    std::shared_ptr<TreeNode> getRoot();

private:
    void cloneNodeRecursive(const std::shared_ptr<TreeNode>& src, std::shared_ptr<TreeNode> dstParent);
    std::unordered_map<QString, std::shared_ptr<TreeNode>> cache;
    std::shared_ptr<TreeNode> root;
};
