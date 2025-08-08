#include "cache.h"
#include "database.h"
#include <QMessageBox>
#include <QUuid>
Cache::Cache() {}

void Cache::loadFromDb(const std::shared_ptr<TreeNode>& dbNode) {
    cache.clear();
    root = std::make_shared<TreeNode>(dbNode->id, dbNode->data);
    cloneNodeRecursive(dbNode, nullptr);
}

void Cache::cloneNodeRecursive(const std::shared_ptr<TreeNode>& src, std::shared_ptr<TreeNode> dstParent) {
    auto node = std::make_shared<TreeNode>(src->id, src->data, dstParent);
    node->isDeleted = src->isDeleted;
    cache[node->id] = node;

    if (dstParent) dstParent->children.push_back(node);
    else root = node;

    for (const auto& child : src->children) {
        cloneNodeRecursive(child, node);
    }
}

std::shared_ptr<TreeNode> Cache::getNode(const QString& id) {
    return cache.count(id) ? cache[id] : nullptr;
}

void Cache::markDeleted(const QString& id) {
    auto node = getNode(id);
    if (!node) return;

    std::function<void(std::shared_ptr<TreeNode>)> markSubtree = [&](std::shared_ptr<TreeNode> n) {
        n->isDeleted = true;
        for (auto& child : n->children) {
            markSubtree(child);
        }
    };
    markSubtree(node);
}

void Cache::addChild(const QString& parentId, const QString& data) {
    auto parent = getNode(parentId);
    if (!parent) {
        qWarning() << "Родительский узел не найден:" << parentId;
        return;
    }

    if (parent->isDeleted) {
        qWarning() << "Попытка добавить дочерний элемент к удаленному узлу:" << parentId;
        return;
    }
    QString newId = QUuid::createUuid().toString();
    auto newNode = std::make_shared<TreeNode>(newId, data, parent);
    parent->children.push_back(newNode);
    cache[newId] = newNode;
}
void Cache::saveToDb(Database& db) {
    for (auto& [id, node] : cache) {
        db.saveNode(node);
    }
}

void Cache::reset() {
    cache.clear();
    root.reset();
}

std::shared_ptr<TreeNode> Cache::getRoot() {
    return root;
}

bool Cache::isNodeDeleted(const QString& id) const {
    auto it = cache.find(id);
    return it != cache.end() && it->second->isDeleted;
}
