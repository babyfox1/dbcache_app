#include "database.h"

Database::Database() {
    createTestData();
}

std::shared_ptr<TreeNode> Database::getNode(const QString& id) {
    if (storage.count(id)) {
        return storage[id];
    }
    return nullptr;
}

void Database::saveNode(const std::shared_ptr<TreeNode>& node) {
    if (!storage.count(node->id)) {
        auto newNode = std::make_shared<TreeNode>(node->id, node->data);
        newNode->isDeleted = node->isDeleted;

        storage[newNode->id] = newNode;

        if (auto parent = node->parent.lock()) {
            if (storage.count(parent->id)) {
                storage[parent->id]->children.push_back(newNode);
                newNode->parent = storage[parent->id];
            }
        }
    }
    else {
        storage[node->id]->data = node->data;
        storage[node->id]->isDeleted = node->isDeleted;
    }

    for (const auto& child : node->children) {
        saveNode(child);
    }
}

void Database::reset() {
    storage.clear();
    createTestData();
}

std::shared_ptr<TreeNode> Database::getRoot() {
    return root;
}

void Database::createTestData() {
    storage.clear();

    root = std::make_shared<TreeNode>("root", "Root");
    storage[root->id] = root;

    auto level1 = std::make_shared<TreeNode>("1", "Level 1", root);
    root->children.push_back(level1);
    storage[level1->id] = level1;

    auto level2 = std::make_shared<TreeNode>("2", "Level 2", level1);
    level1->children.push_back(level2);
    storage[level2->id] = level2;

    auto level3 = std::make_shared<TreeNode>("3", "Level 3", level2);
    level2->children.push_back(level3);
    storage[level3->id] = level3;

    auto level4 = std::make_shared<TreeNode>("4", "Level 4", level3);
    level3->children.push_back(level4);
    storage[level4->id] = level4;
}
