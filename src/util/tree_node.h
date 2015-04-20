#ifndef TREENODE_H
#define TREENODE_H

#include <list>

class TreeNode
{
public:
    typedef std::list<TreeNode *> TreeNodeList;
    typedef TreeNodeList::iterator Itor;
    explicit TreeNode(TreeNode *owner);
    virtual ~TreeNode();
    void freeOrgans();

    void fore();
    void back();

    TreeNode *owner;
    TreeNodeList organs;
};

#endif // TREENODE_H
