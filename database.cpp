#include "database.h"

Database::Database() {
    createTestData();
}

std::shared_ptr<TreeNode> Database::recursive_find(const std::shared_ptr<TreeNode>& node, const QString& id) {
    if (!node) return {};
    if (node->id == id) return node;

    for (const auto& child : std::as_const(node->children)) {
        if (auto found = recursive_find(child, id)) {
            return found;
        }
    }
    return {};
}

std::shared_ptr<TreeNode> Database::getNode(const QString& id) {
    return recursive_find(root, id);
}

void Database::saveNode(const std::shared_ptr<TreeNode>& node) {
    if (!node) return;

    if (node->parent.expired()) {
        root = node;
        node->parent.reset();
        return;
    }

    if (auto parent = getNode(node->parent.lock()->id)) {
        bool exists = std::any_of(parent->children.begin(), parent->children.end(),
                                  [&](const std::shared_ptr<TreeNode>& ch) {
                                      return ch.get() == node.get() || ch->id == node->id;
                                  });
        if (!exists) {
            parent->children.push_back(node);
        }

        node->parent = parent;
        return;
    }
}

void Database::reset() {
    root.reset();
    createTestData();
}

std::shared_ptr<TreeNode> Database::getRoot() {
    return root;
}

void Database::createTestData() {
    root.reset();

    root = std::make_shared<TreeNode>("root", "Root");
    saveNode(root);

    auto level1 = std::make_shared<TreeNode>("1", "Level 1", root);
    level1->parent = root;
    saveNode(level1);

    auto level2 = std::make_shared<TreeNode>("2", "Level 2", level1);
    level2->parent = level1;
    saveNode(level2);

    auto level3 = std::make_shared<TreeNode>("3", "Level 3", level2);
    level3->parent = level2;
    saveNode(level3);

    auto level4 = std::make_shared<TreeNode>("4", "Level 4", level3);
    level4->parent = level3;
    saveNode(level4);
}
