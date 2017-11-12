#include "TreeNode.h"

TreeNode::TreeNode(Box bounding, int child1, int child2, int t1, int t2)
{
    this->bounding = bounding;
    this->child1 = child1;
    this->child2 = child2;
    this->t1 = t1;
    this->t2 = t2;
}
