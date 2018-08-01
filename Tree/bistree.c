#include <stdlib.h>
#include <string.h>
#include <Tree/bistree.h>

//前置声明
static void destroy_right(BisTree *tree, BiTreeNode *node);

//左旋转
static void rotate_left(BiTreeNode **node)
{
	BiTreeNode  *left, *grandchild;
	//取得左结点
	left = bitree_left(*node);

	if(((AvlNode *)bitree_data(left))->factor == AVL_LFT_HEAVY){
		//LL旋转
		bitree_left(*node) = bitree_right(left);
		bitree_right(left) = *node;
		((AvlNode *)bitree_data(*node))->factor = AVL_BALANCED;
		((AvlNode *)bitree_data(left))->factor = AVL_BALANCED;
		*node = left;
	}else{
		//LR旋转
		grandchild = bitree_right(left);
		bitree_right(left) = bitree_left(grandchild);
		bitree_left(grandchild) = left;
		bitree_left(*node) = bitree_right(grandchild);
		bitree_right(grandchild) = *node;

		//设置平衡因子
		switch(((AvlNode *)bitree_data(grandchild))->factor)
		{
			case AVL_LFT_HEAVY:
				((AvlNode *)bitree_data(*node))->factor = AVL_RGT_HEAVY;
				((AvlNode *)bitree_data(left))->factor = AVL_BALANCED;
				break;
			case AVL_BALANCED:
				((AvlNode *)bitree_data(*node))->factor = AVL_BALANCED;
				((AvlNode *)bitree_data(left))->factor = AVL_BALANCED;
				break;
			case AVL_RGT_HEAVY:
				((AvlNode *)bitree_data(*node))->factor = AVL_BALANCED;
				((AvlNode *)bitree_data(left))->factor = AVL_LFT_HEAVY;
				break;
		}

		((AvlNode *)bitree_data(grandchild))->factor = AVL_BALANCED;
		*node = grandchild;
	}

	return;
}

//右旋转,跟左旋转一样
static void rotate_right(BiTreeNode **node)
{
	BiTreeNode  *right, *grandchild;

	right = bitree_right(*node);
	if(((AvlNode *)bitree_data(right))->factor == AVL_RGT_HEAVY){
		//RR旋转
		bitree_right(*node) = bitree_left(right);
		bitree_left(right) = *node;
		((AvlNode *)bitree_data(*node))->factor = AVL_BALANCED;
		((AvlNode *)bitree_data(right))->factor = AVL_BALANCED;
		*node = right;
	}else{
		//RL旋转
		grandchild = bitree_left(right);
		bitree_left(right) = bitree_right(grandchild);
		bitree_right(grandchild) = right;
		bitree_right(*node) = bitree_left(grandchild);
		bitree_left(grandchild) = *node;

		//设置平衡因子
		switch(((AvlNode *)bitree_data(grandchild))->factor)
		{
			case AVL_LFT_HEAVY:
				((AvlNode *)bitree_data(*node))->factor = AVL_BALANCED;
				((AvlNode *)bitree_data(right))->factor = AVL_RGT_HEAVY;
				break;
			case AVL_BALANCED:
				((AvlNode *)bitree_data(*node))->factor = AVL_BALANCED;
				((AvlNode *)bitree_data(right))->factor = AVL_BALANCED;
				break;
			case AVL_RGT_HEAVY:
				((AvlNode *)bitree_data(*node))->factor = AVL_LFT_HEAVY;
				((AvlNode *)bitree_data(right))->factor = AVL_BALANCED;
				break;
		}

		((AvlNode *)bitree_data(grandchild))->factor = AVL_BALANCED;
		*node = grandchild;
	}

	return;
}

//销毁左子树
static void destroy_left(BisTree *tree, BiTreeNode *node)
{
	BiTreeNode	**position;

	//空树直接返回
	if(bitree_size(tree) == 0) { return; }

	if(node == NULL) //删除整个树
		position = &tree->root;
	else //删除node的左子树
		position = &node->left;

	if(*position != NULL)
	{
		//递归删除结点
		destroy_left(tree, *position);
		destroy_right(tree, *position);

		if(tree->destroy != NULL)
		{
			tree->destroy(((AvlNode *)(*position)->data)->data);
		}

		free((*position)->data);
		free(*position);
		*position = NULL;
		tree->size--;
	}

	return;
}

static void destroy_right(BisTree *tree, BiTreeNode *node)
{
	BiTreeNode  **position;

	if(bitree_size(tree) == 0) { return; }

	if(node == NULL) //删除整个树
		position = &tree->root;
	else
		position = &node->right; //删除node的右子树

	if(*position != NULL)
	{
		//递归删除结点
		destroy_left(tree, *position);
		destroy_right(tree, *position);

		if(tree->destroy != NULL)
		{
			tree->destroy(((AvlNode *)(*position)->data)->data);
		}

		free((*position)->data);
		free(*position);
		*position = NULL;
		tree->size--;
	}

	return;
}

static int insert(BisTree *tree, BiTreeNode **node, const void *data, int*balanced)
{
	AvlNode		*avl_data;
	int			cmpval, retval;
	
	if(bitree_is_eob(*node)){ //空结点,表示是个空树
		//分配空间
		if((avl_data = (AvlNode *)malloc(sizeof(AvlNode))) == NULL)
			return -1;

		//初始化avl_data
		avl_data->factor = AVL_BALANCED;
		avl_data->hidden = 0;
		avl_data->data = (void *)data;

		//插入到空树中
		return bitree_ins_left(tree, *node, avl_data);
	}else{
		//compare判断插入元素与node值的大小
		cmpval = tree->compare(data, ((AvlNode *)bitree_data(*node))->data);
		if(cmpval < 0){ //左移动
			if(bitree_is_eob(bitree_left(*node))){ //node左结点为空
				//分配空间
				if((avl_data = (AvlNode *)malloc(sizeof(AvlNode))) == NULL)
					return -1;

				//初始化avl_data
				avl_data->factor = AVL_BALANCED;
				avl_data->hidden = 0;
				avl_data->data = (void *)data;

				if(bitree_ins_left(tree, *node, avl_data) != 0)
					return -1;

				*balanced = 0;
			}else{ //递归插入
				if((retval = insert(tree, &bitree_left(*node), data, balanced))!= 0)
				{
					return retval;
				}
			}
			
			//确认保持平衡
			if(!(*balanced)){
				switch(((AvlNode *)bitree_data(*node))->factor){
					case AVL_LFT_HEAVY:
						rotate_left(node);
						*balanced = 1;
						break;
					case AVL_BALANCED:
						((AvlNode *)bitree_data(*node))->factor = AVL_LFT_HEAVY;
						break;
					case AVL_RGT_HEAVY:
						((AvlNode *)bitree_data(*node))->factor = AVL_BALANCED;
						*balanced = 1;
				}
			}
		}/* if (cmpval < 0) */
		else if(cmpval > 0){ 
			if(bitree_is_eob(bitree_right(*node))){ //右结点为空
				if((avl_data = (AvlNode *)malloc(sizeof(AvlNode))) == NULL)
					return -1;

				avl_data->factor = AVL_BALANCED;
				avl_data->hidden = 0;
				avl_data->data = (void *)data;

				if (bitree_ins_right(tree, *node, avl_data) != 0)
					return -1;

				*balanced = 0;
			}else{
				if((retval = insert(tree, &bitree_right(*node), data, balanced))!= 0)
				{
					return retval;
				}
			}
			
			//保持平衡
			if(!(*balanced)){
				switch(((AvlNode *)bitree_data(*node))->factor){
					case AVL_LFT_HEAVY:
						((AvlNode *)bitree_data(*node))->factor = AVL_BALANCED;
						*balanced = 1;
						break;
					case AVL_BALANCED:
						((AvlNode *)bitree_data(*node))->factor = AVL_RGT_HEAVY;
						break;
					case AVL_RGT_HEAVY:
						rotate_right(node);
						*balanced = 1;
				}
			}
		} /* if (cmpval > 0) */
		else{ //cmpval返回值为0
			if(!((AvlNode *)bitree_data(*node))->hidden){ //非隐藏,直接返回
				return 1;
			}else{ //隐藏
				if(tree->destroy != NULL)
				{
					tree->destroy(((AvlNode *)bitree_data(*node))->data);
				}

				((AvlNode *)bitree_data(*node))->data = (void *)data;
				((AvlNode *)bitree_data(*node))->hidden = 0;
				*balanced = 1;
			}
		}
	}

	return 0;
}

//为data元素标记hidden
static int hide(BisTree *tree, BiTreeNode *node, const void *data)
{

	int		cmpval, retval;
	//node结点不为空
	if(bitree_is_eob(node))  { return -1; }
	//与node结点比较大小
	cmpval = tree->compare(data, ((AvlNode *)bitree_data(node))->data);
	if(cmpval < 0){
		//左移,并递归调用hide
		retval = hide(tree, bitree_left(node), data);
	}else if(cmpval > 0){
		//右移,并递归调用hide
		retval = hide(tree, bitree_right(node), data);
	}else{
		//标记hidden
		((AvlNode *)bitree_data(node))->hidden = 1;
		retval = 0;
	}

	return retval;
}

//二叉搜索树
static int lookup(BisTree *tree, BiTreeNode *node, void **data)
{
	int		cmpval,	retval;

	//结点不为空,返回-1表示查找失败
	if(bitree_is_eob(node)) { return -1; }
	//比较node结点值
	cmpval = tree->compare(*data, ((AvlNode *)bitree_data(node))->data);

	if(cmpval < 0){
		//左移并递归查找
		retval = lookup(tree, bitree_left(node), data);
	}else if(cmpval > 0){
		//右移并递归查找
		retval = lookup(tree, bitree_right(node), data);
	}else{
		if(!((AvlNode *)bitree_data(node))->hidden){
			//如果非隐藏,表示查找成功,返回找到的元素
			*data = ((AvlNode *)bitree_data(node))->data;
			retval = 0;
		}else{
			//查找失败
			return -1;
		}
	}

	return retval;
}

/*          以下是二叉搜索树的操作函数,均是调用函数实现               */

//初始化二叉搜索树
void bistree_init(BisTree *tree, int (*compare)(const void *key1, const void
			*key2), void (*destroy)(void *data))
{
	bitree_init(tree, destroy);
	tree->compare = compare;

	return;
}

//销毁二叉搜索树
void bistree_destroy(BisTree *tree)
{
	destroy_left(tree, NULL);
	memset(tree, 0, sizeof(BisTree));
	return;
}

//二叉搜索树插入元素
int bistree_insert(BisTree *tree, const void *data)
{
	int balanced = 0;
	return insert(tree, &bitree_root(tree), data, &balanced);
}

//二叉搜索树移除元素
int bistree_remove(BisTree *tree, const void *data)
{
	return hide(tree, bitree_root(tree), data);
}

//二叉搜索树查找
int bistree_lookup(BisTree *tree, void **data)
{
	return lookup(tree, bitree_root(tree), data);
}
