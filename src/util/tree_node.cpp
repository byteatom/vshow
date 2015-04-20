#include "tree_node.h"

#include <cassert>
#include <algorithm>

TreeNode::TreeNode(TreeNode *owner) :
    owner{owner}
{
    if (owner) owner->organs.push_back(this);
}

TreeNode::~TreeNode()
{
    if (owner) owner->organs.remove(this);
}

void TreeNode::freeOrgans() {
    auto iter = organs.begin();
    while (iter != organs.end()) {
        TreeNode *node = *iter;
        iter = organs.erase(iter);
        delete node;
    }
}

void TreeNode::fore() {
    if (!owner) return;

    Itor itor = owner->organs.begin();
    while (itor != owner->organs.end()) {
        if (*itor == this) break;
        ++itor;
    }
    if (itor == owner->organs.end()) return;
    Itor itorPre = itor;
    --itorPre;
    if (itorPre == owner->organs.end()) return;
    std::swap(*itorPre, *itor);
}

void TreeNode::back(){
    if (!owner) return;

    Itor itor = owner->organs.begin();
    while (itor != owner->organs.end()) {
        if (*itor == this) break;
        ++itor;
    }
    if (itor == owner->organs.end()) return;
    Itor itorNext = itor;
    ++itorNext;
    if (itorNext == owner->organs.end()) return;
    std::swap(*itor, *itorNext);
}

