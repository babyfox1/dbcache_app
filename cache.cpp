#include "cache.h"
#include "database.h"
#include <QMessageBox>
#include <QUuid>

Cache::Cache() {}

void Cache::loadFromDb(const std::shared_ptr<TreeNode>& dbNode) {
    root.reset();
    if (dbNode) {
        root = std::make_shared<TreeNode>(dbNode->id, dbNode->data);
        cloneNodeRecursive(dbNode, root);
    }
}

void Cache::cloneNodeRecursive(const std::shared_ptr<TreeNode>& src, std::shared_ptr<TreeNode> dstParent) {
    if (!src || !dstParent) return;

    for (const auto& srcChild : src->children) {
        auto newChild = std::make_shared<TreeNode>(srcChild->id, srcChild->data, dstParent);
        newChild->isDeleted = srcChild->isDeleted;
        dstParent->children.push_back(newChild);
        cloneNodeRecursive(srcChild, newChild);
    }
}

std::shared_ptr<TreeNode> Cache::recursiveFind(const std::shared_ptr<TreeNode>& node, const QString& id) const {
    if (!node) return nullptr;
    if (node->id == id) return node;

    for (const auto& child : node->children) {
        if (auto found = recursiveFind(child, id)) {
            return found;
        }
    }
    return nullptr;
}

std::shared_ptr<TreeNode> Cache::getNode(const QString& id) {
    return recursiveFind(root, id);
}

void Cache::markSubtreeDeleted(const std::shared_ptr<TreeNode>& node) {
    if (!node) return;

    node->isDeleted = true;
    for (auto& child : node->children) {
        markSubtreeDeleted(child);
    }
}

void Cache::markDeleted(const QString& id) {
    if (auto node = getNode(id)) {
        markSubtreeDeleted(node);
    }
}

void Cache::addChild(const QString& parentId, const QString& data) {
    auto parent = getNode(parentId);
    if (!parent || parent->isDeleted) {
        return;
    }

    QString newId = QUuid::createUuid().toString();
    auto newNode = std::make_shared<TreeNode>(newId, data, parent);
    parent->children.push_back(newNode);
}

void Cache::saveRecursive(const std::shared_ptr<TreeNode>& node, Database& db) {
    if (!node) return;
    db.saveNode(node);
    for (const auto& child : node->children) {
        saveRecursive(child, db);
    }
}

void Cache::saveToDb(Database& db) {
    saveRecursive(root, db);
}

void Cache::reset() {
    root.reset();
}

std::shared_ptr<TreeNode> Cache::getRoot() {
    return root;
}

bool Cache::isNodeDeleted(const QString& id) const {
    if (auto node = recursiveFind(root, id)) {
        return node->isDeleted;
    }
    return false;
}
