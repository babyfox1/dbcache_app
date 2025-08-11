#pragma once

#include "treenode.h"
#include "database.h"
#include <memory>
#include <QString>

class Cache {
public:
    Cache();
    void loadFromDb(const std::shared_ptr<TreeNode>& dbNode);
    std::shared_ptr<TreeNode> getNode(const QString& id) const;
    bool isNodeDeleted(const QString& id) const;
    void addChild(const QString& parentId, const QString& data);
    void markDeleted(const QString& id);
    void saveToDb(Database& db);
    void reset();

    std::shared_ptr<TreeNode> getRoot() const;

private:
    void saveRecursive(const std::shared_ptr<TreeNode>& node, Database& db);
    std::shared_ptr<TreeNode> recursiveFind(const std::shared_ptr<TreeNode>& node, const QString& id) const;
    void cloneNodeRecursive(const std::shared_ptr<TreeNode>& src, std::shared_ptr<TreeNode> dstParent);
    void markSubtreeDeleted(const std::shared_ptr<TreeNode>& node);
    std::shared_ptr<TreeNode> root;
};
