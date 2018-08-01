#include <stdlib.h>
#include <string.h>
#include <Tree/bitree.h>

//初始化二叉树
void bitree_init(BiTree *tree, void (*destroy)(void *data))
{
	tree->size = 0;
	tree->destroy = destroy;
	tree->root = NULL;

	return;
}

//销毁二叉树
void bitree_destroy(BiTree *tree)
{
	//(递归的方法)移除所有树节点
	bitree_rem_left(tree, NULL);
	//重置为0
	memset(tree, 0, sizeof(BiTree));
	return;
}

int bitree_ins_left(BiTree *tree, BiTreeNode *node, const void *data)
{
	BiTreeNode  *new_node, **position;
	//插入结点
	if(node == NULL){ //如果结点为空,则要插入在树头结点处
		if(bitree_size(tree) > 0)		{ return -1; }
		//插入在头结点
		position = &tree->root;
	}else{
		//如果该结点左结点非空,则插入失败
		if (bitree_left(node) != NULL)	{ return -1; }
		//插入在元素的左结点
		position = &node->left;
	}

	//分配一个二叉树结点空间
	if((new_node = (BiTreeNode *)malloc(sizeof(BiTreeNode))) == NULL)
		return -1;

	//将结点赋值后插入树中
	new_node->data = (void *)data;
	new_node->left = NULL;
	new_node->right = NULL;
	*position = new_node;
	tree->size++;

	return 0;
}

//类似插入左结点操作
int bitree_ins_right(BiTree *tree, BiTreeNode *node, const void *data)
{
	BiTreeNode  *new_node, **position;

	if(node == NULL){
		if(bitree_size(tree) > 0)		{ return -1; }
		position = &tree->root;
	}else{
		if(bitree_right(node) != NULL)	{ return -1; }
		position = &node->right;
	}

	if ((new_node = (BiTreeNode *)malloc(sizeof(BiTreeNode))) == NULL)
		return -1;

	new_node->data = (void *)data;
	new_node->left = NULL;
	new_node->right = NULL;
	*position = new_node;
	tree->size++;

	return 0;
}

//删除树左结点
void bitree_rem_left(BiTree *tree, BiTreeNode *node)
{
	BiTreeNode  **position;
	//树不为空
	if(bitree_size(tree) == 0) { return; }

	if(node == NULL) //结点为空,则表示删除整个二叉树
		position = &tree->root;
	else
		position = &node->left;

	//递归式删除结点
	if(*position != NULL)
	{
		//递归调用删除结点
		bitree_rem_left(tree, *position);
		bitree_rem_right(tree, *position);
		if(tree->destroy != NULL)
		{
			//删除data元素
			tree->destroy((*position)->data);
		}

		free(*position);
		*position = NULL;
		tree->size--;
	}

	return;
}

//删除树右结点,类似删除左结点
void bitree_rem_right(BiTree *tree, BiTreeNode *node)
{
	BiTreeNode  **position;
	//树不为空
	if(bitree_size(tree) == 0)  { return; }
	//结点为空,则表示删除整个二叉树
	if(node == NULL)
		position = &tree->root;
	else
		position = &node->right;

	if(*position != NULL)
	{
		bitree_rem_left(tree, *position);
		bitree_rem_right(tree, *position);

		if(tree->destroy != NULL)
		{
			tree->destroy((*position)->data);
		}

		free(*position);
		*position = NULL;
		tree->size--;
	}

	return;
}

//合并树
int bitree_merge(BiTree *merge, BiTree *left, BiTree *right, const void *data)
{
	//初始化merge
	bitree_init(merge, left->destroy);

	//插入data元素
	if(bitree_ins_left(merge, NULL, data) != 0)
	{
		bitree_destroy(merge);
		return -1;
	}

	//合并树
	bitree_root(merge)->left = bitree_root(left);
	bitree_root(merge)->right = bitree_root(right);
	merge->size = merge->size + bitree_size(left) + bitree_size(right);

	//之前的left和right树置空
	left->root = NULL;
	left->size = 0;
	right->root = NULL;
	right->size = 0;

	return 0;
}
