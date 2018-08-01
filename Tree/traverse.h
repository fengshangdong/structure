#ifndef TRAVERSE_H
#define TRAVERSE_H

#include <Tree/bitree.h>
#include <List/list.h>

int preorder(const BiTreeNode *node, List *list);

int inorder(const BiTreeNode *node, List *list);

int postorder(const BiTreeNode *node, List *list);

#endif
