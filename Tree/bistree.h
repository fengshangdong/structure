#ifndef BISTREE_H
#define BISTREE_H

#include <Tree/bitree.h>

//定义平衡因子
#define		AVL_LFT_HEAVY	1
#define     AVL_BALANCED    0
#define     AVL_RGT_HEAVY	-1

//定义AVL树结点
//@data		数据
//@hidden	隐藏
//@factor	平衡因子
typedef struct AvlNode_{
  void               *data;
  int                hidden;
  int                factor;
}AvlNode;

//二叉树实现二叉搜索
typedef BiTree BisTree;

void bistree_init(BisTree *tree, int (*compare)(const void *key1, const void
      *key2), void (*destroy)(void *data));

void bistree_destroy(BisTree *tree);

int bistree_insert(BisTree *tree, const void *data);

int bistree_remove(BisTree *tree, const void *data);

int bistree_lookup(BisTree *tree, void **data);

#define bistree_size(tree) ((tree)->size)

#endif
