#include "treenode.h"

TreeNode::TreeNode(const QString& id, const QString& data, std::shared_ptr<TreeNode> parent)
    : id(id), data(data), parent(parent) {}
