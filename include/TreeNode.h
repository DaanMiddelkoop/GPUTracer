#ifndef TREENODE_H
#define TREENODE_H

#include <Box.h>

class TreeNode
{
    public:
        TreeNode(Box bounding, int child1, int child2, int t1, int t2);
        Box bounding;
        int child1;
        int child2;
        int t1;
        int t2;

    protected:

    private:
};

#endif // TREENODE_H
