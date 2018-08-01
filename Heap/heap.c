#include <stdlib.h>
#include <string.h>
#include <Heap/heap.h>

//定义宏
#define heap_parent(npos) ((int)(((npos) - 1) / 2))
#define heap_left(npos) (((npos) * 2) + 1)
#define heap_right(npos) (((npos) * 2) + 2)

//初始化堆
void heap_init(Heap *heap, int (*compare)(const void *key1, const void *key2),
		void (*destroy)(void *data))
{
	heap->size = 0;
	heap->compare = compare;
	heap->destroy = destroy;
	heap->tree = NULL;

	return;
}

//销毁堆
void heap_destroy(Heap *heap)
{
	int	i;
	if(heap->destroy != NULL)
	{
		//销毁所有堆元素
		for(i = 0; i < heap_size(heap); i++)
		{
			heap->destroy(heap->tree[i]);
		}
	}

	free(heap->tree);
	memset(heap, 0, sizeof(Heap));
	return;
}

//插入元素
int heap_insert(Heap *heap, const void *data)
{
	void	*temp;
	int     ipos, ppos;

	//分配空间
	if((temp = (void **)realloc(heap->tree, (heap_size(heap) + 1) * sizeof(void *))) == NULL){
		return -1;
	}else{
		heap->tree = temp;
	}
	
	//data插入尾结点
	heap->tree[heap_size(heap)] = (void *)data;

	ipos = heap_size(heap);
	ppos = heap_parent(ipos);

	//新结点向上推
	while(ipos > 0 && heap->compare(heap->tree[ppos], heap->tree[ipos]) < 0)
	{
		//与父节点交换
		temp = heap->tree[ppos];
		heap->tree[ppos] = heap->tree[ipos];
		heap->tree[ipos] = temp;

		ipos = ppos;
		ppos = heap_parent(ipos);
	}

	heap->size++;
	return 0;
}

//取出元素
int heap_extract(Heap *heap, void **data)
{
	void	*save, *temp;
	int     ipos, lpos, rpos, mpos;

	//堆为空则失败
	if(heap_size(heap) == 0) { return -1; }
		
	//取顶点数据
	*data = heap->tree[0];
	//保存最后一个元素
	save = heap->tree[heap_size(heap) - 1];
	//取出顶点元素
	if(heap_size(heap) - 1 > 0){ //堆个数大于1
		//分配空间
		if((temp = (void **)realloc(heap->tree, (heap_size(heap) - 1) * sizeof
						(void *))) == NULL){
			return -1;
		}else{
			heap->tree = temp;
		}

		heap->size--;
	}else{ //堆只有1个元素,取出后堆直接为空
		free(heap->tree);
		heap->tree = NULL;
		heap->size = 0;
		return 0;
	}

	//将尾结点拷贝到顶点
	heap->tree[0] = save;

	//新结点向下推
	ipos = 0;
	lpos = heap_left(ipos);
	rpos = heap_right(ipos);

	while(1){
		lpos = heap_left(ipos);
		rpos = heap_right(ipos);

		//取mpos值
		if(lpos < heap_size(heap) && heap->compare(heap->tree[lpos], heap->tree[ipos]) > 0){
			mpos = lpos;
		}else{
			mpos = ipos;
		}

		if(rpos < heap_size(heap) && heap->compare(heap->tree[rpos], heap->tree[mpos]) > 0)
		{
			mpos = rpos;
		}

		if(mpos == ipos){ //堆内部排序完成
			break;
		}else{
			//交换当前结点与子结点
			temp = heap->tree[mpos];
			heap->tree[mpos] = heap->tree[ipos];
			heap->tree[ipos] = temp;
			//下移
			ipos = mpos;
		}
	}

	return 0;
}
